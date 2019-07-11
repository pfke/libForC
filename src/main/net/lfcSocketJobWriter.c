#include "lfcSocketJobWriter.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include "../core/lfcCore.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocketJobWriter_t *_lfcSocketJobWriter;
static const lfcSocketJobWriter_class_t *_lfcSocketJobWriter_class;
static lfcSocketJobWriter_methods_t _lfcSocketJobWriter_methods isAnUnused_param;

/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcSocketJobWriter_t *public_lfcSocketJobWriter_ctor (void *_self, va_list *app);
static lfcSocketJobWriter_t *public_lfcSocketJobWriter_dtor (lfcSocketJobWriter_t *self);

static int impl_lfcSocketJobWriter__SocketJob__doWork (lfcSocketJob_t *_self);
static bool impl_lfcSocketJobWriter__SocketJob__reset (lfcSocketJob_t *_self);

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
static lfcSocketJobWriter_t *public_lfcSocketJobWriter_ctor(
    void *_self,
    va_list *app
) {
    int fd = va_arg(*app, int);
    void *context = va_arg(*app, void *);
    void *ident = va_arg(*app, void *);
    int timeout_in_s = va_arg(*app, int);

    lfcSocketJobWriter_t *self = (lfcSocketJobWriter_t *) lfcObject_super_ctor(
        lfcSocketJobWriter(), _self,
        fd, context, ident, timeout_in_s, 1
    );
    ASSERT_PTR(self, err_self, "could not instantiate super");

    // read args
    self->buf = va_arg(*app, const char *);
    self->buf_len = va_arg(*app, size_t);
    self->onWriteComplete = va_arg(*app, fn_onWriteComplete_cb);
    ASSERT_PTR(self->buf, err_param_null, "write_buf is NULL");

    return self;

    err_param_null:
    delete(self);
err_self:
    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcSocketJobWriter_t *public_lfcSocketJobWriter_dtor(
    lfcSocketJobWriter_t *self
) {
    lfcObject_super_dtor(lfcSocketJobWriter(), self);

    return self;
}

/**
 * Socket ist bereit -> lesen.
 */
static int impl_lfcSocketJobWriter__SocketJob__doWork(
    lfcSocketJob_t *_self
) {
    ssize_t written_bytes;
    lfcSocketJobWriter_t *self = asInstanceOf(lfcSocketJobWriter(), _self);
    if (!self) { return -EINVAL; }
    if (lfcSocketJob_isEnabled(_self) == false) return 1;

    if (_self->timeout_in_s != -1) {
        fd_set set;
        struct timeval timeout;
        int rv;

        FD_ZERO(&set); /* clear the set */
        FD_SET(_self->fd, &set); /* add our file descriptor to the set */

        timeout.tv_sec = _self->timeout_in_s;
        timeout.tv_usec = 0;

        rv = select(_self->fd + 1, NULL, &set, NULL, &timeout);
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
                    if (self->onWriteComplete) { self->onWriteComplete(self->_.context, self->_.ident, -errno, self->buf); }
                    lfcSocketJob_disable(_self);
                    return -errno;
            }
        } else if(rv == 0) {
            return 0;
        }
    }

    written_bytes = write(self->_.fd, &self->buf[self->already_written], self->buf_len - self->already_written);

    if (written_bytes == -1) {
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
                if (self->onWriteComplete) { self->onWriteComplete(self->_.context, self->_.ident, -errno, self->buf); }
                lfcSocketJob_disable(_self);
                return -errno;
        }
    }

    self->already_written += written_bytes;

    // Ist die scheisse komplett geschrieben?
    if (self->already_written >= self->buf_len) {
        if (self->onWriteComplete) { self->onWriteComplete(self->_.context, self->_.ident, self->buf_len, self->buf); }
        lfcSocketJob_disable(_self);
        return 1;
    }
    return 0;
}

/**
 * Diese Methode setzt den Job zurück auf die Anffangsparameter. D.h. ein Reader liest beim nächsten AUfruf und geht von
 * bisher 0 empfangenen Daten aus.
 *
 * @param self ist die Instanz
 * @return true .. Reset hat funktioniert
 *          false .. Reset hat nicht funktioniert
 */
static bool impl_lfcSocketJobWriter__SocketJob__reset (
    lfcSocketJob_t *_self
) {
    lfcSocketJobWriter_t *self = asInstanceOf(lfcSocketJobWriter(), _self);
    if (!self) return -EINVAL;

    self->already_written = 0;
    _self->current_run = 0;
    lfcSocketJob_enable(_self);

    return true;
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
CLASS_CTOR__START(lfcSocketJobWriter)
    CLASS_CTOR__INIT_SUPER(lfcSocketJobWriter, lfcSocketJob)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocketJobWriter_class_t *lfcSocketJobWriter_class() {
    return _lfcSocketJobWriter_class
           ? _lfcSocketJobWriter_class
           : (_lfcSocketJobWriter_class = (lfcSocketJobWriter_class_t *) new(
            lfcSocketJob_class(), "lfcSocketJobWriter_class", lfcSocketJob_class(), sizeof(lfcSocketJobWriter_class_t),

            lfcObject_ctor, "", impl_lfcSocketJobWriter_class_ctor,

            (void *) 0)
           );
}

const lfcSocketJobWriter_t *lfcSocketJobWriter() {
    return _lfcSocketJobWriter
           ? _lfcSocketJobWriter
           : (_lfcSocketJobWriter = (lfcSocketJobWriter_t *) new(
            lfcSocketJobWriter_class(),
            "lfcSocketJobWriter", lfcSocketJob(), sizeof(lfcSocketJobWriter_t),

            lfcObject_ctor, "ctor", public_lfcSocketJobWriter_ctor,
            lfcObject_dtor, "dtor", public_lfcSocketJobWriter_dtor,

            lfcSocketJob_doWork, "doWork", impl_lfcSocketJobWriter__SocketJob__doWork,
            lfcSocketJob_reset, "reset", impl_lfcSocketJobWriter__SocketJob__reset,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocketJobWriter);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Einen neuen Writer erstellen
 *
 * @param fd der zu schreibende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param write_buf zu schreibender Buffer
 * @param len_to_write zu schreibende Länge
 * @param Callback der aufgerufen wird, wenn der Schreib-Zugriff fertig ist
 * @param timeout_in_s nach wieviel s soll das Schreiben abgebrochen werden? (0 == kein timeout)
 * @return die erstellte Instanz
 */
lfcSocketJobWriter_t *lfcSocketJobWriter_ctor (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    const char *write_buf,
    size_t len_to_write,
    fn_onWriteComplete_cb onWriteComplete
) {
    return (lfcSocketJobWriter_t *)new(lfcSocketJobWriter(), fd, context, ident, timeout_in_s, write_buf, len_to_write, onWriteComplete);
}
