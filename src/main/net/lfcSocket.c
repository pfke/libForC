#include "lfcSocket.h"
#include <stdlib.h>
#include <asm/errno.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>

#include "lfcSocketJobAcceptConn.h"


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcSocket, lfcObject,
    int,     getFd,       (),
    int,     listen,      (void *, fn_onAcceptConn_cb),
    ssize_t, read,        (char *, size_t, int),
    ssize_t, read_async,  (char *, size_t, int, unsigned int, fn_onReadComplete_cb),
    ssize_t, read_job,    (lfcSocketJobReader_t *),
    ssize_t, write,       (const char *, size_t, int),
    ssize_t, write_async, (const char *, size_t, int, fn_onWriteComplete_cb),
    ssize_t, write_job,   (lfcSocketJobWriter_t *)
)


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

static void private_lfcSocket_onAcceptConn(
    void *context,
    void *ident,
    ssize_t read_len isAnUnused_param,
    char *read_buf isAnUnused_param
) {
    lfcSocket_t *self = asInstanceOf(lfcSocket(), context);
    if (!self) { return; }

    // server-Anfrage: Socket annehmen
    struct sockaddr pin;
    socklen_t addrlen = sizeof(pin);

    // wir müssen die Verbindung annehmen, sonst ist sie für ewig pending
    int fd = accept(self->fd, &pin, &addrlen);
    if(fd == -1) {
//        lfcLogger_log_ERR(
//            self->_.logger,
//            "%s: accept lieferte einen Fehler zurück: %d / '%s'", __func__,
//            errno, strerror(errno)
//        );
        return;
    }

    fn_onAcceptConn_cb callback = lfcHashmap_get(self->listen_requestMap, ident);

    if (callback) {
        callback(self, lfcSocket_ctor(self->socketHandler, fd), context);
    }

//    lfcLogger_log_INFO(
//        self->_.logger,
//        "%s: neue Verbindungsanfrage bekommen (endpoint-fd=%d) und als fd %d angenommen", __func__,
//        self->endpoint_socket->fd, fd
//    );
}


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

    self->listen_requestMap = LFCHASH__CREATE_PTRKEY_HASHMAP();
    ASSERT_PTR(self->listen_requestMap, err_reqMap, "failed to alloc listen request map");

    return self;

err_reqMap:
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
    lfcSocketHandler_killall_waitFor(self->socketHandler, self->fd, 1000);

    delete(self->listen_requestMap);

    close(self->fd);

    return lfcObject_super_dtor(lfcSocket(), self);
}

/**
 * Aktiviert ein Listen der Verbindung.
 */
static int public_lfcSocket_getFd(
    lfcSocket_t *self
) {
    return self->fd;
}

/**
 * Aktiviert ein Listen der Verbindung.
 */
static int public_lfcSocket_listen(
    lfcSocket_t *self,
    void *context,
    fn_onAcceptConn_cb onAcceptConn_cb
) {
    if (listen(self->fd, SOMAXCONN)) {
        return NULL;
    } else {
        lfcHashmap_put(self->listen_requestMap, context, onAcceptConn_cb);

        return public_lfcSocket_read_job(
            self,
            asInstanceOf(
                lfcSocketJobReader(),
                lfcSocketJobAcceptConn_ctor(
                    self->fd,
                    self,
                    context,
                    private_lfcSocket_onAcceptConn
                ))
        );
    }
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

    void wait_for_mutex(void *context __attribute__((unused)), void *ident __attribute__((unused)), ssize_t read_len, char *read_buf __attribute__((unused))) {
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

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_ctor (
    lfcSocketHandler_t *socketHandler,
    int fd
) {
    return (lfcSocket_t *)new(lfcSocket(), socketHandler, fd);
}

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_connect_tcpStream(
    lfcSocketHandler_t *socketHandler,
    const char *node, const char *port
) {
    int32_t sd = -1;

    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;
    struct addrinfo* res = NULL;

    //
    int err = getaddrinfo(node, port, &hints, &res);
    if (err) {
        fprintf(stderr, "failed to resolve local socket address (err=%d / '%s')\n", err, gai_strerror(err));
        return NULL;
    }

    // create socket
    if ((sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        goto err;
    }

    // SO_REUSEADDR should be routinely set for TCP server sockets in order to allow the network service to be restarted when there are connections in the ESTABLISHED or TIME-WAIT state:
    int reuseaddr = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
        goto err;
    }

    // bind the socket to the port number
    if (connect(sd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "failed to connect (err=%d / '%s')\n", errno, strerror(errno));
        goto err;
    }

    freeaddrinfo(res);

    return lfcSocket_ctor(socketHandler, sd);

err:
    if (sd < 0) { close(sd); }

    return NULL;
}

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_listenFor(
    lfcSocketHandler_t *socketHandler,
    int sock_family, int sock_socktype, int sock_protocol,
    const struct sockaddr *sock_addr, socklen_t sock_addrlen,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
) {
    int32_t sd = -1;

    // create socket
    if ((sd = socket(sock_family, sock_socktype, sock_protocol)) == -1) {
        goto err;
    }

    // SO_REUSEADDR should be routinely set for TCP server sockets in order to allow the network service to be restarted when there are connections in the ESTABLISHED or TIME-WAIT state:
    int reuseaddr = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
        goto err;
    }

    // bind the socket to the port number
    if (bind(sd, sock_addr, sock_addrlen) == -1) {
        fprintf(stderr, "failed to bind (err=%d / '%s')\n", errno, strerror(errno));
        goto err;
    }

    lfcSocket_t *sock = lfcSocket_ctor(socketHandler, sd);

    if (lfcSocket_listen(sock, context, onAcceptConn_cb)) {
        delete(sock);
        sock = NULL;
    }

    return sock;

err:
    if (sd < 0) { close(sd); }

    return NULL;
}

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_listenFor_tcpStream(
    lfcSocketHandler_t *socketHandler,
    const char *node, const char *port,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
) {
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;
    struct addrinfo* res = NULL;

    //
    int err = getaddrinfo(node, port, &hints, &res);
    if (err) {
        fprintf(stderr, "failed to resolve local socket address (err=%d / '%s')\n", err, gai_strerror(err));
        return NULL;
    }

    lfcSocket_t *sock = lfcSocket_listenFor(
        socketHandler,
        res->ai_family, res->ai_socktype, res->ai_protocol,
        res->ai_addr, res->ai_addrlen,
        context, onAcceptConn_cb
    );

    freeaddrinfo(res);

    return sock;
}

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_listenFor_unixDomain(
    lfcSocketHandler_t *socketHandler,
    const char *node, int socketType,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
) {
    if (socketType != SOCK_STREAM && socketType != SOCK_SEQPACKET) {
        return NULL;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, node, sizeof(addr.sun_path) - 1);

    lfcSocket_t *sock = lfcSocket_listenFor(
        socketHandler,
        AF_UNIX, socketType, 0,
        (struct sockaddr *)&addr, sizeof(addr),
        context, onAcceptConn_cb
    );

    return sock;
}

/**
 */
lfcSocket_t *lfcSocket_listenFor_unixDomain_asStream(
    lfcSocketHandler_t *socketHandler,
    const char *node,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
) {
    return lfcSocket_listenFor_unixDomain(socketHandler, node, SOCK_STREAM, context, onAcceptConn_cb);
}

/**
 */
lfcSocket_t *lfcSocket_listenFor_unixDomain_asSeqPacket(
    lfcSocketHandler_t *socketHandler,
    const char *node,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
) {
    return lfcSocket_listenFor_unixDomain(socketHandler, node, SOCK_SEQPACKET, context, onAcceptConn_cb);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
