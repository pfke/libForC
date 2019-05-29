#include "lfcSocket.h"
#include <stdlib.h>
#include <asm/errno.h>
#include <errno.h>


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocket_t *_lfcSocket;
static const lfcSocket_class_t *_lfcSocket_class;
static lfcSocket_methods_t _lfcSocket_methods isAnUnused_param;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcSocket_t *public_lfcSocket_ctor (void *_self, va_list *app);
static lfcSocket_t *public_lfcSocket_dtor (lfcSocket_t *self);

static ssize_t public_lfcSocket_read (lfcSocket_t *self, char *buf, size_t buf_size, int timeout);
static ssize_t public_lfcSocket_read_async (lfcSocket_t *self, char *buf, size_t buf_size, int timeout, unsigned int repeat, fn_onReadComplete_cb onReadComplete);
static ssize_t public_lfcSocket_read_job (lfcSocket_t *self, lfcSocketJobReader_t *job);

static ssize_t public_lfcSocket_write (lfcSocket_t *self, const char *buf, size_t buf_size, int timeout);
static ssize_t public_lfcSocket_write_async (lfcSocket_t *self, const char *buf, size_t buf_size, int timeout, fn_onWriteComplete_cb onWriteComplete);
static ssize_t public_lfcSocket_write_job (lfcSocket_t *self, lfcSocketJobWriter_t *job);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcSocket_t *public_lfcSocket_ctor (
    void *_self,
    va_list *app
) {
    lfcSocket_t *self = (lfcSocket_t *) lfcObject_super_ctor(lfcSocket(), _self);

    ASSERT_PTR(self, err_self, "could not create instance");

    // read args
    self->socketHandler = va_arg(*app, lfcSocketHandler_t *);
    self->fd = va_arg(*app, int);

    fcntl(self->fd, F_SETFL, O_NONBLOCK);

    ASSERT_PTR(isInstanceOf(lfcSocketHandler(), self->socketHandler), err_sockHandler, "SocketHandler-Instance invalid");

    return self;

//err_add_sock:
err_sockHandler:
    delete(self);
err_self:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcSocket_t *public_lfcSocket_dtor (
    lfcSocket_t *self
) {
    lfcObject_super_dtor(lfcSocket(), self);

    lfcSocketHandler_killall_waitFor(self->socketHandler, self->fd, 1000);

    return self;
}

/**
 * Lesen von Daten über den Socket.
 * Diese Funktion blockiert, bis entweder alle Daten (<code>buf_size</code>) gelesen wurde oder ein Fehler auftratt.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @return -1 .. Fehler beim Lesen (errno is set)
 *                  EACCES .. Bereich konnte nicht geschützt werden
 *                  EINVAL .. Parameter ungültig
 *         >=0 Anzahl der gelesenen Daten
 */
static ssize_t public_lfcSocket_read (
    lfcSocket_t *self,
    char *buf,
    size_t buf_size,
    int timeout
) {
    ssize_t ret_len = -1;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    lfcSocketJobReader_t *job;

    if (!buf) {
        errno = EINVAL;
        return -1;
    }
    if (buf_size == 0) return 0;

    if (pthread_mutex_lock(&mutex)) {
        errno = EACCES;
        return -1;
    }

    void wait_for_mutex(void *context, void *ident, ssize_t read_len, char *read_buf) {
        ret_len = read_len;
        pthread_mutex_unlock(&mutex);
    }

    // lesen anwerfen und auf unseren mutex laufen
    job = lfcSocketJobReader_ctor_wRepeat (
        self->fd,
        &mutex,
        &ret_len,
        0,
        1,
        buf,
        buf_size,
        false,
        wait_for_mutex
     );

    ret_len = lfcSocketHandler_read (
        self->socketHandler, job
    );
    if (ret_len) {
        // die scheisse canceln, job disablen
        lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

        pthread_mutex_unlock(&mutex);
        return ret_len;
    }

    if (timeout == -1) {
        if (pthread_mutex_lock(&mutex)) {
            // die scheisse canceln, job disablen
            lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

            errno = EACCES;
            return -1;
        }
    } else {
        struct timespec abstime_timeout;

        clock_gettime(CLOCK_REALTIME, &abstime_timeout);

        abstime_timeout.tv_sec += timeout;

        if (pthread_mutex_timedlock(&mutex, &abstime_timeout)) {
            // die scheisse canceln, job disablen
            lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

            return 0;
        }
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    // die scheisse canceln, job disablen
    lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

    errno = 0;
    return ret_len;
}

/**
 * Lesen von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @param repeat Angabe, wie oft der Lese-Befehl ausgeführt werden soll (0 .. endlos, 1 .. one-shot, >1 .. Anzahl)
 * @param onReadComplete Callback, wenn das Lesen fertig ist
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
static ssize_t public_lfcSocket_read_async (
    lfcSocket_t *self,
    char *buf,
    size_t buf_size,
    int timeout,
    unsigned int repeat,
    fn_onReadComplete_cb onReadComplete
) {
    return public_lfcSocket_read_job(
        self,
        lfcSocketJobReader_ctor_wRepeat (
            self->fd,
            NULL,
            NULL,
            timeout,
            repeat,
            buf,
            buf_size,
            false,
            onReadComplete
        )
    );
}

/**
 * Lesen von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job Lese-Auftrag
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
static ssize_t public_lfcSocket_read_job (
    lfcSocket_t *self,
    lfcSocketJobReader_t *job
) {
    return lfcSocketHandler_read (
        self->socketHandler,
        job
    );
}

/**
 * Schreiben von Daten über den Socket.
 * Diese Funktion blockiert, bis entweder alle Daten geschrieben wurde oder beim Schreiben ein Fehler auftratt.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @return -1 .. Fehler beim Schreiben (errno is set)
 *         >=0 Anzahl der geschriebenen Daten
 */
static ssize_t public_lfcSocket_write (
    lfcSocket_t *self,
    const char *buf,
    size_t buf_size,
    int timeout
) {
    ssize_t ret_len;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    lfcSocketJobWriter_t *job;

    if (!buf) {
        errno = EINVAL;
        return -1;
    }
    if (buf_size == 0) return 0;

    if (pthread_mutex_lock(&mutex)) {
        errno = EACCES;
        return -1;
    }

    // lesen anwerfen und auf unseren mutex laufen
    job = lfcSocketJobWriter_ctor(
        self->fd,
        NULL, NULL,
        timeout,
        buf, buf_size,
        lambda(void, (void *context isAnUnused_param, void *ident isAnUnused_param, ssize_t written_len, const char *write_buf isAnUnused_param) {
            ret_len = written_len;
            pthread_mutex_unlock(&mutex);
        })
    );
    if (!job) { return -1; }
    ret_len = lfcSocketHandler_write (
        self->socketHandler, job
    );
    if (ret_len) {
        // die scheisse canceln, job disablen
        lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

        pthread_mutex_unlock(&mutex);
        return ret_len;
    }

    if (timeout == -1) {
        if (pthread_mutex_lock(&mutex)) {
            // die scheisse canceln, job disablen
            lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

            errno = EACCES;
            return -1;
        }
    } else {
        struct timespec abstime_timeout;

        clock_gettime(CLOCK_REALTIME, &abstime_timeout);
        abstime_timeout.tv_sec += timeout;

        if (pthread_mutex_timedlock(&mutex, &abstime_timeout)) {
            // die scheisse canceln, job disablen
            lfcSocketJob_disable(asInstanceOf(lfcSocketJob(), job));

            errno = ETIMEDOUT;
            return -1;
        }
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    errno = 0;
    return ret_len;
}

/**
 * Schreiben von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @param onWriteComplete Callback, wenn das Schreiben fertig ist
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
static ssize_t public_lfcSocket_write_async (
    lfcSocket_t *self,
    const char *buf,
    size_t buf_size,
    int timeout,
    fn_onWriteComplete_cb onWriteComplete
) {
    return public_lfcSocket_write_job (
        self,
        lfcSocketJobWriter_ctor(
            self->fd,
            NULL, NULL,
            timeout,
            buf, buf_size,
            onWriteComplete
        )
    );
}

/**
 * Schreiben von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job Schreib-Auftrag
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
static ssize_t public_lfcSocket_write_job (
    lfcSocket_t *self,
    lfcSocketJobWriter_t *job
) {
    return lfcSocketHandler_write (
        self->socketHandler,
        job
    );
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
CLASS_CTOR__START(lfcSocket)
        OVERRIDE_METHOD(lfcSocket, read)
        OVERRIDE_METHOD(lfcSocket, read_async)
        OVERRIDE_METHOD(lfcSocket, read_job)
        OVERRIDE_METHOD(lfcSocket, write)
        OVERRIDE_METHOD(lfcSocket, write_async)
        OVERRIDE_METHOD(lfcSocket, write_job)
    CLASS_CTOR__INIT_SUPER(lfcSocket, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocket_class_t *lfcSocket_class() {
    return _lfcSocket_class
           ? _lfcSocket_class
           : (_lfcSocket_class = (lfcSocket_class_t *) new(
            lfcObject_class(), "lfcSocket_class", lfcObject_class(), sizeof(lfcSocket_class_t),

            lfcObject_ctor, "", impl_lfcSocket_class_ctor,

            (void *) 0)
           );
}

const lfcSocket_t *lfcSocket() {
    return _lfcSocket
           ? _lfcSocket
           : (_lfcSocket = (lfcSocket_t *) new(
            lfcSocket_class(),
            "lfcSocket", lfcObject(), sizeof(lfcSocket_t),

            lfcObject_ctor, "ctor", public_lfcSocket_ctor,
            lfcObject_dtor, "dtor", public_lfcSocket_dtor,

            lfcSocket_read, "read", public_lfcSocket_read,
            lfcSocket_read_async, "read_async", public_lfcSocket_read_async,
            lfcSocket_read_job, "read_job", public_lfcSocket_read_job,

            lfcSocket_write, "write", public_lfcSocket_write,
            lfcSocket_write_async, "write_async", public_lfcSocket_write_async,
            lfcSocket_write_job, "write_job", public_lfcSocket_write_job,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocket);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_IMPL_ACCESSOR(lfcSocket, read, ssize_t, char *, size_t, int);
lfcOOP_IMPL_ACCESSOR(lfcSocket, read_async, ssize_t, char *, size_t, int, unsigned int, fn_onReadComplete_cb);
lfcOOP_IMPL_ACCESSOR(lfcSocket, read_job, ssize_t, lfcSocketJobReader_t *);
lfcOOP_IMPL_ACCESSOR(lfcSocket, write, ssize_t, const char *, size_t, int);
lfcOOP_IMPL_ACCESSOR(lfcSocket, write_async, ssize_t, const char *, size_t, int, fn_onWriteComplete_cb);
lfcOOP_IMPL_ACCESSOR(lfcSocket, write_job, ssize_t, lfcSocketJobWriter_t *);

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_ctor (
    lfcSocketHandler_t *socketHandler,
    int fd
) {
    return (lfcSocket_t *)new(lfcSocket(), socketHandler, fd);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
