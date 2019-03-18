#include "lfcSocketJobReaderStream.h"

#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include "../core/lfcCore.h"
#include <sys/socket.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocketJobReaderStream_t *_lfcSocketJobReaderStream;
static const lfcSocketJobReaderStream_class_t *_lfcSocketJobReaderStream_class;
static lfcSocketJobReaderStream_methods_t _lfcSocketJobReaderStream_methods isAnUnused_param;

/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcSocketJobReaderStream_t *public_lfcSocketJobReaderStream_ctor (void *_self, va_list *app);
static lfcSocketJobReaderStream_t *public_lfcSocketJobReaderStream_dtor (lfcSocketJobReaderStream_t *self);

static int impl_lfcSocketJobReaderStream__SocketJob__doWork (lfcSocketJob_t *_self);

/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Constructor der Liste selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcSocketJobReaderStream_t *public_lfcSocketJobReaderStream_ctor(
    void *_self,
    va_list *app
) {
    int fd = va_arg(*app, int);
    void *context = va_arg(*app, void *);
    void *ident = va_arg(*app, void *);
    int timeout_in_s = va_arg(*app, int);
    char *buf =  va_arg(*app, char *);
    size_t buf_len = va_arg(*app, size_t);
    int free_buf_on_exit = va_arg(*app, int);
    fn_onReadComplete_cb onReadComplete = va_arg(*app, fn_onReadComplete_cb);

    lfcSocketJobReaderStream_t *self = (lfcSocketJobReaderStream_t *) lfcObject_super_ctor(
        lfcSocketJobReaderStream(), _self,
        fd, context, ident, timeout_in_s, 0,
        buf, buf_len, onReadComplete
    );
    ASSERT_PTR(self, err_self, "could not instantiate super");

    self->free_buf_on_exit = free_buf_on_exit;

    return self;

err_self:
    return NULL;
}

/**
 * Destructor.
 */
static lfcSocketJobReaderStream_t *public_lfcSocketJobReaderStream_dtor(
    lfcSocketJobReaderStream_t *self
) {
    lfcObject_super_dtor(lfcSocketJobReaderStream(), self);

    if (self->free_buf_on_exit) free(self->_.buf);

    return self;
}

/**
 * Socket ist bereit -> lesen.
 */
static int impl_lfcSocketJobReaderStream__SocketJob__doWork (
    lfcSocketJob_t *_self
) {
    ssize_t read_bytes;
    lfcSocketJobReaderStream_t *self = asInstanceOf(lfcSocketJobReaderStream(), _self);
    if (!self) return -EINVAL;
    if (lfcSocketJob_isEnabled(_self) == false) return 1;

    if (_self->timeout_in_s != -1) {
        fd_set set;
        struct timeval timeout;
        int rv;

        FD_ZERO(&set); /* clear the set */
        FD_SET(_self->fd, &set); /* add our file descriptor to the set */

        timeout.tv_sec = _self->timeout_in_s;
        timeout.tv_usec = 0;

        rv = select(_self->fd + 1, &set, NULL, NULL, &timeout);
        if(rv == -1) {
            switch (errno) {
                case EBADF: // The fildes argument is not a valid file descriptor open for reading.
                case EBADMSG: // The file is a STREAM file that is set to control-normal mode and the message waiting to be read includes a control part.
                case EINVAL: // The STREAM or multiplexer referenced by fildes is linked (directly or indirectly) downstream from a multiplexer.
                case ECONNRESET: // A read was attempted on a socket and the connection was forcibly closed by its peer.
                case ENOTCONN: // A read was attempted on a socket that is not connected.
                case EIO: // A physical I/O error has occurred.
                case ENOBUFS: // Insufficient resources were available in the system to perform the operation.
                case ENOMEM: // Insufficient memory was available to fulfill the request.
                default:
                    // other error -> close that fd
                    self->_.onReadComplete(self->_._.context, self->_._.ident, -errno, self->_.buf);
                    lfcSocketJob_disable(_self);
                    return -errno;
            }
        } else if(rv == 0) {
            return 0;
        }
    }

    read_bytes = read(self->_._.fd, &self->_.buf[self->_.already_read], self->_.buf_len - self->_.already_read);

    if (read_bytes == -1) {
//        fprintf(stderr, "%d@%s: errno=%d/%s\n", __LINE__, __func__, errno, strerror(errno));
        switch (errno) {
            case EAGAIN: // The file is a socket, the O_NONBLOCK flag is set for the file descriptor, and the thread would be delayed in the read operation.
            case EINTR:  // The read operation was terminated due to the receipt of a signal, and no data was transferred.
            case ETIMEDOUT: // A read was attempted on a socket and a transmission timeout occurred.
                return 0;

            case EBADF: // The fildes argument is not a valid file descriptor open for reading.
            case EBADMSG: // The file is a STREAM file that is set to control-normal mode and the message waiting to be read includes a control part.
            case EINVAL: // The STREAM or multiplexer referenced by fildes is linked (directly or indirectly) downstream from a multiplexer.
            case ECONNRESET: // A read was attempted on a socket and the connection was forcibly closed by its peer.
            case ENOTCONN: // A read was attempted on a socket that is not connected.
            case EIO: // A physical I/O error has occurred.
            case ENOBUFS: // Insufficient resources were available in the system to perform the operation.
            case ENOMEM: // Insufficient memory was available to fulfill the request.
            default:
                // other error -> close that fd
                self->_.onReadComplete(self->_._.context, self->_._.ident, -errno, self->_.buf);
                lfcSocketJob_disable(_self);
                return -errno;
        }
    } else if (read_bytes == 0) {
        // gegenstelle ist zu
        return -1;
    }

    // Ist die scheisse komplett gelesen?
    if (read_bytes > 0) {
        self->_.onReadComplete(self->_._.context, self->_._.ident, read_bytes, self->_.buf);
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
CLASS_CTOR__START(lfcSocketJobReaderStream)
    CLASS_CTOR__INIT_SUPER(lfcSocketJobReaderStream, lfcSocketJobReader)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocketJobReaderStream_class_t *lfcSocketJobReaderStream_class() {
    return _lfcSocketJobReaderStream_class
           ? _lfcSocketJobReaderStream_class
           : (_lfcSocketJobReaderStream_class = (lfcSocketJobReaderStream_class_t *) new(
            lfcSocketJobReader_class(), "lfcSocketJobReaderStream_class", lfcSocketJobReader_class(), sizeof(lfcSocketJobReaderStream_class_t),

            lfcObject_ctor, "", impl_lfcSocketJobReaderStream_class_ctor,

            (void *) 0)
           );
}

const lfcSocketJobReaderStream_t *lfcSocketJobReaderStream() {
    return _lfcSocketJobReaderStream
           ? _lfcSocketJobReaderStream
           : (_lfcSocketJobReaderStream = (lfcSocketJobReaderStream_t *) new(
            lfcSocketJobReaderStream_class(),
            "lfcSocketJobReaderStream", lfcSocketJobReader(), sizeof(lfcSocketJobReaderStream_t),

            lfcObject_ctor, "ctor", public_lfcSocketJobReaderStream_ctor,
            lfcObject_dtor, "dtor", public_lfcSocketJobReaderStream_dtor,

            lfcSocketJob_doWork, "doWork", impl_lfcSocketJobReaderStream__SocketJob__doWork,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocketJobReaderStream);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Einen neuen Reader erstellen
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param len_to_read zu lesende Länge
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @param timeout_in_s nach wieviel s soll das Lesen abgebrochen werden? (0 == kein timeout)
 * @return die erstellte Instanz
 */
lfcSocketJobReaderStream_t *lfcSocketJobReaderStream_ctor (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    size_t len_to_read,
    fn_onReadComplete_cb onReadComplete
) {
    return lfcSocketJobReaderStream_ctor_wBuffer (
        fd,
        context,
        ident,
        timeout_in_s,
        calloc(1, len_to_read),
        len_to_read, 1,
        onReadComplete
    );
}

/**
 * Einen neuen Reader erstellen, der Lese-Buffer wird von uns alloziiert und dann im Callback zurueckgegeben.
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param read_buf in diesen Buffer soll gelesen werden
 * @param len_to_read zu lesende Länge
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @param timeout_in_s nach wieviel s soll das Lesen abgebrochen werden? (0 == kein timeout)
 * @return die erstellte Instanz
 */
lfcSocketJobReaderStream_t *lfcSocketJobReaderStream_ctor_wBuffer (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    char *read_buf,
    size_t len_to_read,
    int free_buf_on_exit,
    fn_onReadComplete_cb onReadComplete
) {
    if (!read_buf) { return NULL; }
    if (!len_to_read) { return NULL; }

    return (lfcSocketJobReaderStream_t *)new(
        lfcSocketJobReaderStream(),
        fd,
        context, ident,
        timeout_in_s,
        read_buf, len_to_read, free_buf_on_exit,
        onReadComplete
    );
}
