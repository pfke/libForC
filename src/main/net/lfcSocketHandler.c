#include "lfcSocketHandler.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "../threading/lfcThreadMisc.h"

#include "lfcSocketJobAcceptConn.h"
#include "lfcSocketJobReader.h"
#include "lfcSocketJobReaderStream.h"
#include "lfcSocketJobWriter.h"


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/

#define LNSH_CMDPIPE_MSG_SIZE       (sizeof(lfcSocketHandler_job_buf_t))
typedef enum lfcSocketHandler_cmdPipeIdent lfcSocketHandler_cmdPipeIdent_e;
typedef enum lfcSocketHandler_cmdPipeCmd lfcSocketHandler_cmdPipeCmd_e isAnUnused_param;

#define LNSH_CMDPIPE_FROMUSER_THREAD       0
#define LNSH_CMDPIPE_FROMUSER_NOTHREAD     1

enum lfcSocketHandler_cmdPipeIdent {
    LNSH_CMDPIPE_IDENT_IS_CMD,
    LNSH_CMDPIPE_IDENT_IS_READ,
    LNSH_CMDPIPE_IDENT_IS_WRITE,
    LNSH_CMDPIPE_IDENT_IS_KILLFD,
    LNSH_CMDPIPE_IDENT_IS_KILLFD_wMUTEX,
};

enum lfcSocketHandler_cmdPipeCmd {
    LNSH_CMDPIPE_CMD_EXIT,
};

#define lfcSocketHandler_cmdPipe_killall_wMutex_MAGIC       0x0123456789abcdef
typedef struct lfcSocketHandler_cmdPipe_killall_wMutex {
    long long int magic;
    int fd;
    pthread_mutex_t *mutex;
} lfcSocketHandler_cmdPipe_killall_wMutex_t;

typedef struct lfcSocketHandler_job_buf {
    void *job;
    enum lfcSocketHandler_cmdPipeIdent type;
} lfcSocketHandler_job_buf_t;


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocketHandler_t *_lfcSocketHandler;
static const lfcSocketHandler_class_t *_lfcSocketHandler_class;
static lfcSocketHandler_methods_t _lfcSocketHandler_methods isAnUnused_func;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/

static int priv_lfcSocketHandler_addJob(lfcSocketHandler_t *self, lfcSocketJob_t *job);
static int priv_lfcSocketHandler_delJob(lfcSocketHandler_t *self, lfcSocketJob_t *job);
static int priv_lfcSocketHandler_removeFd (lfcSocketHandler_t *self, int fd_to_remove);

static void priv_lfcSocketHandler_onCmdPipeReader_readComplete_cb (void *context, void *ident, ssize_t read_len, char *read_buf);
static int priv_lfcSocketHandler_createCmdPipeReaderJob (lfcSocketHandler_t *self);

static int priv_lfcSocketHandler_readSocket (lfcSocketHandler_t *self, int fd);
static int priv_lfcSocketHandler_writeSocket (lfcSocketHandler_t *self, int fd);
static void *priv_lfcSocketHandler_thread (void *context);
static int priv_lfcSocketHandler_sendCmdPipe (lfcSocketHandler_t *self, lfcSocketHandler_cmdPipeIdent_e cmd, void *data);

static lfcSocketHandler_t *public_lfcSocketHandler_ctor (void *_self, va_list *app);
static lfcSocketHandler_t *public_lfcSocketHandler_dtor (lfcSocketHandler_t *self);

static int public_lfcSocketHandler_read (lfcSocketHandler_t *self, lfcSocketJobReader_t *job);
static int public_lfcSocketHandler_write (lfcSocketHandler_t *self, lfcSocketJobWriter_t *job);

static pthread_mutex_t *public_lfcSocketHandler_killall (lfcSocketHandler_t *self, int fd);
static int public_lfcSocketHandler_killall_waitFor (lfcSocketHandler_t *self, int fd, long timeout_in_ms);

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

/**
 * Den angegebenen Job der Liste hinzufuegen und dem epoll anmelden, wenn nötig.
 *
 * @param self ist unsere Instanz
 * @param job der hinzuzufuegende Auftrag
 * @return 0 .. alles ok
 *          -EINVAL .. param null
 *          -EEXIST .. Auftrag existiert bereits in der active Liste
 *          -EAGAIN .. es gibt bereits einen Auftrag gleicher Art in der Liste (lesend oder schreibend)
 *          -EACCES .. could not job add to active-list
 */
static int priv_lfcSocketHandler_addJob(
    lfcSocketHandler_t *self,
    lfcSocketJob_t *job
) {
    struct epoll_event epoll_change;
    int result;

    if (!job) { return -EINVAL; }

    memset(&epoll_change, 0, sizeof(epoll_change));

    // check if exists
    void *found = lfcIIterable_find(self->pending_jobs, lambda_int((void *item isAnIncognito_param) {
        return item == job;
    }));
    if (found) { return -EEXIST; }

    // weitere epolls auf dem fd suchen
    lfcIIterable_foreach(self->pending_jobs, lambda_void((void *item) {
        lfcSocketJob_t *iter = asInstanceOf(lfcSocketJob(), item);

        if (iter && (iter->fd == job->fd)) {
            epoll_change.events |= (isInstanceOf(lfcSocketJobReader(), iter)) ? EPOLLIN : EPOLLOUT;
        }
    }));

    // und add der Liste
    if (lfcList_add(self->pending_jobs, job)) {
        return -EACCES;
    }

    enum EPOLL_EVENTS our_event = (isInstanceOf(lfcSocketJobReader(), job)) ? EPOLLIN : EPOLLOUT;
    // gibt es bereits einen epoll mit gleicher Richtung
    if (epoll_change.events & our_event) {
        return 0;
    }

    // dem epoll hinzufuegen
    int epoll_op = (epoll_change.events) ? EPOLL_CTL_MOD : EPOLL_CTL_ADD; // add or mod?

    epoll_change.events |= our_event;
    epoll_change.data.fd = job->fd;

    result = epoll_ctl(self->epoll_fd, epoll_op, job->fd, &epoll_change);
    if (result) {
        fprintf(stderr, "%d@%s: errno=%d / %s (fd=%d)\n", __LINE__, __func__, errno, strerror(errno), job->fd);
    }

    return result;
}

/**
 * Den angegebenen Job von der Liste loeschen und dem epoll abmelden, wenn noetig.
 *
 * @param self ist unsere Instanz
 * @param job der zu loeschende Auftrag
 * @return 0 .. alles ok
 *          -EINVAL .. param null
 *          -ENOENT .. Auftrag existiert nicht in der active Liste
 */
static int priv_lfcSocketHandler_delJob(
    lfcSocketHandler_t *self,
    lfcSocketJob_t *job
) {
    struct epoll_event epoll_change;
    int fd;

    if (!job) { return -EINVAL; }

    memset(&epoll_change, 0, sizeof(epoll_change));

    // und rm der Liste
    if (lfcList_remove(self->pending_jobs, job)) {
        return -ENOENT;
    }

    // weitere epolls auf dem fd suchen
    lfcIIterable_foreach(self->pending_jobs, lambda_void((void *item) {
        lfcSocketJob_t *iter = asInstanceOf(lfcSocketJob(), item);

        if (iter && (iter != job) && (iter->fd == job->fd)) {
            epoll_change.events |= (isInstanceOf(lfcSocketJobReader(), iter)) ? EPOLLIN : EPOLLOUT;
        }
    }));

    // gibt es noch ein ähnlichen job?
    enum EPOLL_EVENTS our_event = (isInstanceOf(lfcSocketJobReader(), job)) ? EPOLLIN : EPOLLOUT;
    if (epoll_change.events & our_event) {
        delete(job);
        return 0;
    }

    // dem epoll hinzufuegen
    int epoll_op = (epoll_change.events) ? EPOLL_CTL_MOD : EPOLL_CTL_DEL; // del or mod?
    if (epoll_op == EPOLL_CTL_MOD) {
        epoll_change.data.fd = job->fd;
    }

    fd = job->fd;
    delete(job);

    return epoll_ctl(self->epoll_fd, epoll_op, fd, &epoll_change);
}

/**
 * Diese Funktion entfernt den fd (einen oder ggf. mehrere Jobs) aus dem aktuellem Lauschangriff.
 */
static int priv_lfcSocketHandler_removeFd (
    lfcSocketHandler_t *self,
    int fd_to_remove
) {
    lfcSocketJob_t *job;

    if (NULL == self) { return -EINVAL; }

search_again:
    job = lfcIIterable_find(
        self->pending_jobs,
        lambda_int((void *item isAnUnused_param) {
                       lfcSocketJob_t *it = asInstanceOf(lfcSocketJob(), item);

                       if (!it) return 0;

                       return it->fd == fd_to_remove;
                   }));

    if (job) {
        priv_lfcSocketHandler_delJob(self, job);
        goto search_again;
    }

    return 0;
}

/**
 * Read via cmd pipe ist komplett -> Auswertung
 */
static void priv_lfcSocketHandler_onCmdPipeReader_readComplete_cb (
    void *context,
    void *ident isAnUnused_param,
    ssize_t read_len isAnUnused_param,
    char *read_buf
) {
    lfcSocketHandler_t *self = asInstanceOf(lfcSocketHandler(), context);
    if (!self) return;

    lfcSocketHandler_job_buf_t *job_buf = (lfcSocketHandler_job_buf_t *)read_buf;

    lfcSocketHandler_cmdPipeIdent_e cmd = job_buf->type;
    switch (cmd) {
        case LNSH_CMDPIPE_IDENT_IS_CMD:
            switch ((enum lfcSocketHandler_cmdPipeCmd)job_buf->job) {
                case LNSH_CMDPIPE_CMD_EXIT:
                    set_int8_wLocking(&self->thread_is_running, &self->thread_global_mutex, 0);
                    break;

                default:
                    break;
            }
            break;

        case LNSH_CMDPIPE_IDENT_IS_READ:
        case LNSH_CMDPIPE_IDENT_IS_WRITE:
            priv_lfcSocketHandler_addJob(self, asInstanceOf (lfcSocketJob(), job_buf->job));
            return;

        case LNSH_CMDPIPE_IDENT_IS_KILLFD:
        {
            int fd_to_kill = (int)job_buf->job;

            priv_lfcSocketHandler_removeFd(self, fd_to_kill);
        }
            return;

        case LNSH_CMDPIPE_IDENT_IS_KILLFD_wMUTEX:
        {
            int fd_to_kill;
            lfcSocketHandler_cmdPipe_killall_wMutex_t *cmd_obj = (lfcSocketHandler_cmdPipe_killall_wMutex_t*)job_buf->job;
            if (NULL == cmd_obj || cmd_obj->magic != lfcSocketHandler_cmdPipe_killall_wMutex_MAGIC) { return; }

            fd_to_kill = cmd_obj->fd;

            priv_lfcSocketHandler_removeFd(self, fd_to_kill);

            if (NULL != cmd_obj) {
                if (pthread_mutex_trylock(cmd_obj->mutex) == EBUSY) {
                    /* mutex noch gelocked, also unlocken, frei geben macht hauptthread */
                    pthread_mutex_unlock(cmd_obj->mutex);
                } else {
                    /* STR: nicht ganz sauber aber der andere Thread muss eigentlich per timeout raus sein und hat deshalb unlocked */
                    pthread_mutex_unlock(cmd_obj->mutex);
                    free(cmd_obj->mutex);
                    fprintf(stderr, "ERROR: lfcSocketHandler@%s:%d: late free mutex\n", __func__, __LINE__);
                }
                free(cmd_obj);
            }
        }
            return;

        default: return;
    }
}

/**
 * Create cmd pipe readjob
 */
static int priv_lfcSocketHandler_createCmdPipeReaderJob (
    lfcSocketHandler_t *self
) {
    void *buf = calloc(1, LNSH_CMDPIPE_MSG_SIZE);

    if (!buf) { return -ENOMEM; }

    self->cmd_pipe_readJob = lfcSocketJobReader_ctor_wRepeat(
        self->thread_cmd_pipe[LNSH_CMDPIPE_FROMUSER_THREAD],
        self, NULL, 0, 0,
        buf, LNSH_CMDPIPE_MSG_SIZE, true,
        priv_lfcSocketHandler_onCmdPipeReader_readComplete_cb
    );
    if (!self->cmd_pipe_readJob) { return -ENOMEM; }

    return priv_lfcSocketHandler_addJob(self, asInstanceOf(lfcSocketJob(), self->cmd_pipe_readJob));
}

/**
 * Epoll gab ein read auf den übergebenen file descr -> also lesen wir mal
 *
 * @param self
 * @param fd
 * @return -1 .. kein read job mit dem fd gefunden
 */
static int priv_lfcSocketHandler_readSocket (
    lfcSocketHandler_t *self,
    int fd
) {
    // richtigen fd suchen
    lfcSocketJob_t *found = asInstanceOf(
        lfcSocketJob(),
        lfcIIterable_find(
            self->pending_jobs,
            lambda_int((void *item isAnIncognito_param) {
                           lfcSocketJob_t *it = asInstanceOf(lfcSocketJob(), item);
                           if (!isInstanceOf(lfcSocketJobReader(), item)) { return 0; }

                           return (lfcSocketJob_getFD(it) == fd) && (true == lfcSocketJob_isEnabled(it));
                       }))
    );
    if (!found) {
        // aus dem epoll loeschen
        struct epoll_event epoll_change;

        memset(&epoll_change, 0, sizeof(epoll_change));
        epoll_change.events = EPOLLIN;

        int epoll_op = lfcIIterable_find(
            self->pending_jobs,
            lambda_int((void *item isAnIncognito_param) {
                return isInstanceOf(lfcSocketJobWriter(), item);
            })) ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

        epoll_ctl(self->epoll_fd, epoll_op, fd, &epoll_change);

        return -1;
    }

    if (lfcSocketJob_doWork(found) != 0) {
        priv_lfcSocketHandler_delJob(self, found);
    }

    return 0;
}

/**
 * Epoll gab ein write auf den übergebenen file descr -> also schreiben wir mal
 */
static int priv_lfcSocketHandler_writeSocket (
    lfcSocketHandler_t *self,
    int fd
) {
    // richtigen fd suchen
    lfcSocketJob_t *found = asInstanceOf(
        lfcSocketJob(),
        lfcIIterable_find(
            self->pending_jobs,
            lambda_int((void *item isAnIncognito_param) {
                           lfcSocketJob_t *it = asInstanceOf(lfcSocketJob(), item);
                           if (!isInstanceOf(lfcSocketJobWriter(), item)) { return 0; }

                           return (lfcSocketJob_getFD(it) == fd) && (true == lfcSocketJob_isEnabled(it));
                       }))
    );
    if (!found) {
        // aus dem epoll loeschen
        struct epoll_event epoll_change;

        memset(&epoll_change, 0, sizeof(epoll_change));
        epoll_change.events = EPOLLOUT;

        int epoll_op = lfcIIterable_find(
            self->pending_jobs,
            lambda_int((void *item isAnIncognito_param) {
                return isInstanceOf(lfcSocketJobReader(), item);
            })) ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

        epoll_ctl(self->epoll_fd, epoll_op, fd, &epoll_change);

        return -1;
    }

    if (lfcSocketJob_doWork(found) != 0) {
        priv_lfcSocketHandler_delJob(self, found);
    }

    return 0;
}

/**
 * Kommando via cmd-pipe senden.
 */
static int priv_lfcSocketHandler_sendCmdPipe (
    lfcSocketHandler_t *self,
    lfcSocketHandler_cmdPipeIdent_e cmd,
    void *data
) {
    lfcSocketHandler_job_buf_t bufee;

    bufee.type = cmd;
    bufee.job = data;

    if (pthread_mutex_lock(&self->cmd_pipe_write_mutex)) {
        return -EDEADLOCK;
    }

    // return 1 in case of an error, otherwise 0
    ssize_t result = write(self->thread_cmd_pipe[LNSH_CMDPIPE_FROMUSER_NOTHREAD], &bufee, LNSH_CMDPIPE_MSG_SIZE) != LNSH_CMDPIPE_MSG_SIZE;
    pthread_mutex_unlock(&self->cmd_pipe_write_mutex);

    return result;
}

/**
 * Das ist unser Haupt-Thread.
 * Initialisierung vom epoll usw.
 */
static void *priv_lfcSocketHandler_thread (
    void *context
) {
    lfcSocketHandler_t *self = asInstanceOf(lfcSocketHandler(), context);
    struct epoll_event epoll_result;

    if (!self) {
        goto err;
    }

    lfcThreadMisc_setName("sockH");

    self->epoll_fd = epoll_create1(0);
    if (self->epoll_fd == -1) {
        //ALOG_ERROR("epoll_create1 returned an error: %d/%s", errno, strerror(errno));
        goto err_epoll_create;
    }
    memset(&epoll_result, 0, sizeof(epoll_result));

    if (priv_lfcSocketHandler_createCmdPipeReaderJob(self)) {
        goto err_cmdp_read_job;
    }

    set_int8_wLocking(&self->thread_is_running, &self->thread_global_mutex, 1);

    // we are running -> bekanntgeben
    set_int8_sendSignal(&self->thread_start_error_code, 1, &self->thread_global_mutex, &self->thread_is_startet_cond);

    // run the main queue
    while (get_int8_wLocking(&self->thread_is_running, &self->thread_global_mutex)) {
        int epoll_return;

        memset(&epoll_result, 0, sizeof(epoll_result));
        epoll_return = epoll_wait(self->epoll_fd, &epoll_result, 1, 1000); // w/ 1s timeout

        if (epoll_return < 0) {
            switch (errno) {
                case EBADF:
                    //ALOG_ERROR("returned EBADF: 'epoll_fd (%d)' is not a valid file descriptor", epoll_fd);
                    goto exit_this_thread;
                case EFAULT:
                    //ALOG_ERROR("returned EFAULT: The memory area pointed to by 'epoll_result' is not accessible with write permissions.");
                    goto exit_this_thread;
                case EINTR:
                    //ALOG_WARN("returned EINTR: The call was interrupted by a signal handler before either (1) any of the requested events occurred or (2) the timeout expired; see signal(7).");
                    break;
                case EINVAL:
                    //ALOG_ERROR("returned EINVAL: 'epoll_fd' is not an epoll file descriptor, or maxevents is less than or equal to zero.");
                    goto exit_this_thread;

                default:
                    break;
            }

            continue;
        } else if (0 == epoll_return) {
            // timeout, lass mal die jobs prüfen und deaktiverte wegwerfen
            lfcSocketJob_t *job;

search_again:
            job = lfcIIterable_find(
                self->pending_jobs,
                lambda(int, (void *item isAnIncognito_param) {
                    lfcSocketJob_t *it = asInstanceOf(lfcSocketJob(), item);

                    if (!it) return 0;

                    return !lfcSocketJob_isEnabled(it);
                }));
            if (job) {
                priv_lfcSocketHandler_delJob(self, job);
                goto search_again;
            }

            continue;
        }

//        fprintf(stderr, "%d@%s: %s epoll_result.data.fd=%d\n", __LINE__, __func__, (epoll_result.events & EPOLLIN) ? "IN" : "OUT", epoll_result.data.fd);

        // na dann mal lesen und so
        if (epoll_result.events & EPOLLIN) {
            priv_lfcSocketHandler_readSocket(self, epoll_result.data.fd);
        } else if (epoll_result.events & EPOLLOUT) { // oder schreiben
            priv_lfcSocketHandler_writeSocket(self, epoll_result.data.fd);
        } else if (epoll_result.events & EPOLLHUP) {
            // den socket killen
            priv_lfcSocketHandler_removeFd(self, epoll_result.data.fd);
        }
    }

    close(self->epoll_fd);
    pthread_detach(pthread_self());
    return 0;

err_cmdp_read_job:
    close(self->epoll_fd);
err_epoll_create:
err:
    set_int8_sendSignal(&self->thread_start_error_code, 2, &self->thread_global_mutex, &self->thread_is_startet_cond);
    exit_this_thread:
    pthread_detach(pthread_self());

    return (void*)-1;
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcSocketHandler_t *public_lfcSocketHandler_ctor (
    void *_self,
    va_list *app isAnUnused_param
) {
    lfcSocketHandler_t *self = (lfcSocketHandler_t *) lfcObject_super_ctor(lfcSocketHandler(), _self);
    pthread_attr_t thread_attr;

    ASSERT_PTR(self, err_self, "could not create instance");

    ASSERT_PTR((self->pending_jobs = lfcList_ctor()),                   err_job_queue,      "could not create job queue");
    ASSERT_INT0(pthread_attr_init(&thread_attr),                        err_attr_init,      "'pthread_attr_init' failed (result=%d/%s)'\n", result, strerror(result));
    ASSERT_INT0(pthread_mutex_init(&self->thread_global_mutex, NULL),   err_global_mutex,   "'pthread_mutex_init(&self->read_mutex)' failed (result=%d/%s)\n", result, strerror(result));
    ASSERT_INT0(pthread_mutex_init(&self->cmd_pipe_write_mutex, NULL),  err_cmd_pipe_mutex, "'pthread_mutex_init(&self->cmd_pipe_write_mutex)' failed (result=%d/%s)\n", result, strerror(result));
    ASSERT_INT0(pthread_cond_init(&self->thread_is_startet_cond, NULL), err_thread_cond,    "'pthread_cond_init(&self->thread_cond)' failed (result=%d/%s)\n", result, strerror(result));

    ASSERT_INT0(pipe(self->thread_cmd_pipe), err_pipe, "'pipe(self->thread_cmd_pipe)' failed (result=%d/%s)\n", result, strerror(result));

    self->thread_start_error_code = 0;

    // und los gehts
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    ASSERT_INT0(pthread_create(&self->thread, &thread_attr, priv_lfcSocketHandler_thread, (void *)self), err_thread, "'pthread_create(&self->thread)' failed (result=%d/%s)\n", result, strerror(result));

    // warten bis der thread gestartet wurde
    get_int8_awaitTimedSignal(&self->thread_start_error_code, &self->thread_global_mutex, &self->thread_is_startet_cond, 30000);
    if (self->thread_start_error_code != 1) {
        goto err_thread_error;
    }

    pthread_attr_destroy(&thread_attr);
    return self;

err_thread_error:
    set_int8_wLocking(&self->thread_is_running, &self->thread_global_mutex, 0);
    pthread_join(self->thread, NULL);
err_thread:
    close(self->thread_cmd_pipe[0]);
    close(self->thread_cmd_pipe[1]);
err_pipe:
    pthread_cond_destroy(&self->thread_is_startet_cond);
err_thread_cond:
    pthread_mutex_destroy(&self->cmd_pipe_write_mutex);
err_cmd_pipe_mutex:
    pthread_mutex_destroy(&self->thread_global_mutex);
err_global_mutex:
    pthread_attr_destroy(&thread_attr);
err_attr_init:
    delete(self->pending_jobs);
err_job_queue:
    delete(self);
err_self:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcSocketHandler_t *public_lfcSocketHandler_dtor (
    lfcSocketHandler_t *self
) {
    priv_lfcSocketHandler_sendCmdPipe(self, LNSH_CMDPIPE_IDENT_IS_CMD, (void *)LNSH_CMDPIPE_CMD_EXIT);
    pthread_join(self->thread, NULL);

    close(self->thread_cmd_pipe[0]);
    close(self->thread_cmd_pipe[1]);

    // alle pending jobs als Fehler auslösen und löschen
    {
        lfcSocketJob_t *it;
        while ((it = (lfcSocketJob_t *)lfcIIterable_last(self->pending_jobs))) {
            priv_lfcSocketHandler_delJob(self, it);
        }
    }
    delete(self->pending_jobs);

    pthread_cond_destroy(&self->thread_is_startet_cond);
    pthread_mutex_destroy(&self->thread_global_mutex);
    pthread_mutex_destroy(&self->cmd_pipe_write_mutex);

    return lfcObject_super_dtor(lfcSocketHandler(), self);
}

/**
 * Lesen von Daten über den Socket.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job ist der Lese-Job mit all seinen Beschreibungen
 * @return 0 .. ok, Lesen konnte eingequeued werden
 *          -EINVAL .. Parameter fehlerhaft
 */
static int public_lfcSocketHandler_read (
    lfcSocketHandler_t *self,
    lfcSocketJobReader_t *job
) {
    if (!job) { return -EINVAL; }

    return priv_lfcSocketHandler_sendCmdPipe (
        self,
        LNSH_CMDPIPE_IDENT_IS_READ,
        job
    );
}

/**
 * Schreiben von Daten über den Socket.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job ist der Schreib-Job mit all seinen Beschreibungen
 * @return 0 .. ok, Lesen konnte eingequeued werden
 *          -EINVAL .. Parameter fehlerhaft
 */
static int public_lfcSocketHandler_write (
    lfcSocketHandler_t *self,
    lfcSocketJobWriter_t *job
) {
    if (!job) { return -EINVAL; }

    return priv_lfcSocketHandler_sendCmdPipe (
        self,
        LNSH_CMDPIPE_IDENT_IS_WRITE,
        job
    );
}

/**
 * Löschen des angegebenen FDs aus dem Handler.
 * Die Funktion gibt einen Mutex zurück, der freigegeben wird, sobald ALLE Jobs die auf dem fd lauschen gelöscht werden
 * konnten.
 * Der Aufrufer versucht dann ein <code>pthread_mutex_timedlock</code> mit einer individuellen timeout-Erwartung.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param fd zu entfernender FD
 * @return  !NULL .. ok, Mutex der freigegeben wird, sobald vom Thread alle Jobs gelöscht wurden sind
 *          NULL .. Fehler, (aber der FD wird auf jeden Fall rausgenommen, es gibt halt nur kein Mutex zur Bestätigung)
 */
static pthread_mutex_t *public_lfcSocketHandler_killall (
    lfcSocketHandler_t *self,
    int fd
) {
    pthread_mutex_t *return_mutex = NULL;
    lfcSocketHandler_cmdPipe_killall_wMutex_t *cmd;

    /**
     * Das ganze Mutex-gelocke und so ist obsolet, sobald der Lösch-Befehl dem eigenem Thread kommt.
     * Hier kann man den Mist dann direkt löschen.
     */
    if (pthread_self() == self->thread) {
        priv_lfcSocketHandler_removeFd(self, fd);

        return NULL;
    }

    /**
     * Erstmal einen Mutex erstellen den der Thread dann freigeben kann, wenn er alle Jobs gelöscht hat.
     *
     * Wenn das nicht klappt, dann muss es halt ohne Mutex gehen.
     */
    return_mutex = calloc(1, sizeof(pthread_mutex_t));
    if (NULL != return_mutex) {
        pthread_mutex_init(return_mutex, NULL);

        pthread_mutex_lock(return_mutex);
    }

    /**
     * Speicher für die Beschreibung holen, also damit der Thread auch weiss welchen fd er rausnehmen soll und
     * welchen Mutex er dann wieder freigeben muss.
     *
     * Wenn das nicht klappt ist das schlimm - richtig schlimm. Dann müssen wir halt nur die fds killen.
     */
    cmd = calloc(1, sizeof(lfcSocketHandler_cmdPipe_killall_wMutex_t));
    if (NULL == cmd) {
        if (return_mutex) {
            free(return_mutex);
            return_mutex = NULL;
        }

        /**
         * nur das reine kill-Commando an den Thread schicken
         */
        priv_lfcSocketHandler_sendCmdPipe (
            self,
            LNSH_CMDPIPE_IDENT_IS_KILLFD,
            (void *)(intptr_t)fd
        );
    } else {
        cmd->magic = lfcSocketHandler_cmdPipe_killall_wMutex_MAGIC;
        cmd->fd = fd;
        cmd->mutex = return_mutex;

        /**
         * das erweiterte kill-Commando an den Thread schicken
         */
        priv_lfcSocketHandler_sendCmdPipe (
            self,
            LNSH_CMDPIPE_IDENT_IS_KILLFD_wMUTEX,
            cmd
        );
    }

    return return_mutex;
}

/**
 * Löschen des angegebenen FDs aus dem Handler.
 * Und warten auf Bestätigung des Löschens.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param fd zu entfernender FD
 * @param timeout_in_ms Timeout in milli seconds
 * @return  0 .. ok, Mutex der freigegeben wird, sobald vom Thread alle Jobs gelöscht wurden sind
 *          <0 .. Fehler
 */
static int public_lfcSocketHandler_killall_waitFor (
    lfcSocketHandler_t *self,
    int fd,
    long int timeout_in_ms
) {
    int result = 0;
    pthread_mutex_t *mutex = public_lfcSocketHandler_killall(self, fd);

    if (mutex) {
        struct timespec time;
        if (clock_gettime(CLOCK_REALTIME, &time) == 0) {
            timeout_in_ms = 30000;
            time.tv_sec += timeout_in_ms/1000;
            result = pthread_mutex_timedlock(mutex, &time);
        } else 
        {
            fprintf(stderr, "ERROR: lfcSocketHandler@%s:%d: gettime_err\n", __func__, __LINE__);
            /*--- timeout_in_ms = timeout_in_ms; ---*/ 
            result = pthread_mutex_lock(mutex);
        }
        if (result != 0) {
            pthread_mutex_unlock(mutex);
            fprintf(stderr, "ERROR: lfcSocketHandler@%s:%d: errno=%d / %s\n", __func__, __LINE__, result, strerror(result));
        } else {
            /*--- fprintf(stderr, "lfcSocketHandler@%s:%d: free mutex\n", __func__, __LINE__); ---*/
            free(mutex);
        }

    }

    return result;
}

/**
 * Ausgabe aller relevanten Daten auf fp.
 *
 * @param self ist unsere Instanz
 * @param fp hierhin wird geschrieben
 * @return 0 .. ok
 *          <0 .. Fehler
 */
static int public_lfcSocketHandler_printSupportData (
    lfcSocketHandler_t *self,
    FILE *fp
) {
    if (!fp) { return -EINVAL; }

    const char *getJobType (lfcSocketJob_t* in) {
        if (isInstanceOf(lfcSocketJobAcceptConn(), in)) { return "lfcSocketJobAcceptConn"; }
        if (isInstanceOf(lfcSocketJobReaderStream(), in)) { return "lfcSocketJobReaderStream"; }
        if (isInstanceOf(lfcSocketJobReader(), in)) { return "lfcSocketJobReader"; }
        if (isInstanceOf(lfcSocketJobWriter(), in)) { return "lfcSocketJobWriter"; }

        return "unknown";
    }

    fprintf(fp, "      epoll_fd = %d\n", self->epoll_fd);

    if (!pthread_mutex_lock(&self->thread_global_mutex)) {
        size_t no_of_pending_jobs = lfcIIterable_count(self->pending_jobs);
        size_t current_job = 0;

        fprintf(fp, "      no of pending jobs = %zu\n", no_of_pending_jobs);

        lfcIIterable_foreach(self->pending_jobs, lambda_void((void *item) {
            lfcSocketJob_t *iter = asInstanceOf(lfcSocketJob(), item);

            fprintf(fp,
                "       job %zu %s [%s] fd=%d / repeat=%d, run=%d / timeout=%d / context=%p, ident=%p\n",
                current_job,
                iter->enabled ? "ENABLED " : "DISABLED",
                getJobType(iter),
                iter->fd,
                iter->repeat,
                iter->current_run,
                iter->timeout_in_s,
                iter->context,
                iter->ident
            );

            current_job++;
        }));

        pthread_mutex_unlock(&self->thread_global_mutex);
    }

    return 0;
}


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcSocketHandler)
        OVERRIDE_METHOD(lfcSocketHandler, read)
        OVERRIDE_METHOD(lfcSocketHandler, write)
        OVERRIDE_METHOD(lfcSocketHandler, killall)
        OVERRIDE_METHOD(lfcSocketHandler, killall_waitFor)
        OVERRIDE_METHOD(lfcSocketHandler, printSupportData)
    CLASS_CTOR__INIT_SUPER(lfcSocketHandler, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocketHandler_class_t *lfcSocketHandler_class() {
    return _lfcSocketHandler_class
           ? _lfcSocketHandler_class
           : (_lfcSocketHandler_class = (lfcSocketHandler_class_t *) new(
            lfcObject_class(), "lfcSocketHandler_class", lfcObject_class(), sizeof(lfcSocketHandler_class_t),

            lfcObject_ctor, "", impl_lfcSocketHandler_class_ctor,

            (void *) 0)
           );
}

const lfcSocketHandler_t *lfcSocketHandler() {
    return _lfcSocketHandler
           ? _lfcSocketHandler
           : (_lfcSocketHandler = (lfcSocketHandler_t *) new(
            lfcSocketHandler_class(),
            "lfcSocketHandler", lfcObject(), sizeof(lfcSocketHandler_t),

            lfcObject_ctor, "ctor", public_lfcSocketHandler_ctor,
            lfcObject_dtor, "dtor", public_lfcSocketHandler_dtor,

            lfcSocketHandler_read, "read", public_lfcSocketHandler_read,
            lfcSocketHandler_write, "write", public_lfcSocketHandler_write,

            lfcSocketHandler_killall, "killall", public_lfcSocketHandler_killall,
            lfcSocketHandler_killall_waitFor, "killall_waitFor", public_lfcSocketHandler_killall_waitFor,

            lfcSocketHandler_printSupportData, "printSupportData", public_lfcSocketHandler_printSupportData,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocketHandler);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_accessor(lfcSocketHandler, read, int, lfcSocketJobReader_t *);
lfcOOP_accessor(lfcSocketHandler, write, int, lfcSocketJobWriter_t *);
lfcOOP_accessor(lfcSocketHandler, killall, pthread_mutex_t*, int);
lfcOOP_accessor(lfcSocketHandler, killall_waitFor, int, int, long);
lfcOOP_accessor(lfcSocketHandler, printSupportData, int, FILE *);

/**
 * Gibt die lfcSocketHandler Instanz zurück.
 */
lfcSocketHandler_t *lfcSocketHandler_ctor () {
    return (lfcSocketHandler_t *)new(lfcSocketHandler());
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
