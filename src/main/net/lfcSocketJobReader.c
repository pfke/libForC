#include "lfcSocketJobReader.h"

#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include "../core/lfcCore.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocketJobReader_t *_lfcSocketJobReader;
static const lfcSocketJobReader_class_t *_lfcSocketJobReader_class;
static lfcSocketJobReader_methods_t _lfcSocketJobReader_methods isAnUnused_param;

/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcSocketJobReader_t *public_lfcSocketJobReader_ctor (void *_self, va_list *app);
static lfcSocketJobReader_t *public_lfcSocketJobReader_dtor (lfcSocketJobReader_t *self);

static int impl_lfcSocketJobReader__SocketJob__doWork (lfcSocketJob_t *_self);
static bool impl_lfcSocketJobReader__SocketJob__reset (lfcSocketJob_t *_self);

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
static lfcSocketJobReader_t *public_lfcSocketJobReader_ctor(
    void *_self,
    va_list *app
) {
    int fd = va_arg(*app, int);
    void *context = va_arg(*app, void *);
    void *ident = va_arg(*app, void *);
    int timeout_in_s = va_arg(*app, int);
    size_t repeat = va_arg(*app, unsigned int);

    lfcSocketJobReader_t *self = (lfcSocketJobReader_t *) lfcObject_super_ctor(
        lfcSocketJobReader(), _self,
        fd, context, ident, timeout_in_s, repeat
    );
    ASSERT_PTR(self, err_self, "could not instantiate super");

    // read args
    self->buf =  va_arg(*app, char *);
    self->buf_len = va_arg(*app, size_t);
    bool free_buf = va_arg(*app, int) ? true : false;
    self->onReadComplete = va_arg(*app, fn_onReadComplete_cb);
    ASSERT_PTR(self->buf, err_no_mem, "no buf given");
    ASSERT_PTR(self->onReadComplete, err_param_null, "onReadComplete is NULL");
    self->free_buf = free_buf;

    memset(self->buf, 0, self->buf_len);

    return self;

err_param_null:
err_no_mem:
    delete(self);
err_self:
    if (self->buf && self->free_buf) { free(self->buf); }

    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcSocketJobReader_t *public_lfcSocketJobReader_dtor(
    lfcSocketJobReader_t *self
) {
    if (self->buf && self->free_buf) { free(self->buf); }

    return lfcObject_super_dtor(lfcSocketJobReader(), self);
}

/**
 * Socket ist bereit -> lesen.
 */
static int impl_lfcSocketJobReader__SocketJob__doWork (
    lfcSocketJob_t *_self
) {
    ssize_t read_bytes;
    lfcSocketJobReader_t *self = asInstanceOf(lfcSocketJobReader(), _self);
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
                    self->onReadComplete(self->_.context, self->_.ident, -errno, self->buf);
                    lfcSocketJob_disable(_self);
                    return -errno;
            }
        } else if(rv == 0) {
            return 0;
        }
    }

    read_bytes = read(self->_.fd, &self->buf[self->already_read], self->buf_len - self->already_read);

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
                self->onReadComplete(self->_.context, self->_.ident, -errno, self->buf);
                lfcSocketJob_disable(_self);
                return -errno;
        }
    }

    self->already_read += read_bytes;

    // Ist die scheisse komplett gelesen?
    if (self->already_read >= self->buf_len) {
        self->onReadComplete(self->_.context, self->_.ident, self->already_read, self->buf);

        // repeat-counter erniedrigen
        if (_self->repeat == 0) {
            // _self->repeat = _self->repeat; // so lassen
        } else if (_self->repeat == 1) {
            _self->current_run = 1;
//            _self->repeat = UINT32_MAX;
        } else {
            _self->current_run++;
        }

        // wiederholen?
        if (_self->repeat > 0 && _self->current_run >= _self->repeat) {
            // fertsch
            lfcSocketJob_disable(_self);
            return 1;
        } else {
            self->already_read = 0;
            memset(self->buf, 0, self->buf_len);

            return 0;
        }
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
static bool impl_lfcSocketJobReader__SocketJob__reset (
    lfcSocketJob_t *_self
) {
    lfcSocketJobReader_t *self = asInstanceOf(lfcSocketJobReader(), _self);
    if (!self) return -EINVAL;

    self->already_read = 0;
    memset(self->buf, 0, self->buf_len);
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
CLASS_CTOR__START(lfcSocketJobReader)
    CLASS_CTOR__INIT_SUPER(lfcSocketJobReader, lfcSocketJob)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocketJobReader_class_t *lfcSocketJobReader_class() {
    return _lfcSocketJobReader_class
           ? _lfcSocketJobReader_class
           : (_lfcSocketJobReader_class = (lfcSocketJobReader_class_t *) new(
            lfcSocketJob_class(), "lfcSocketJobReader_class", lfcSocketJob_class(), sizeof(lfcSocketJobReader_class_t),

            lfcObject_ctor, "", impl_lfcSocketJobReader_class_ctor,

            (void *) 0)
           );
}

const lfcSocketJobReader_t *lfcSocketJobReader() {
    return _lfcSocketJobReader
           ? _lfcSocketJobReader
           : (_lfcSocketJobReader = (lfcSocketJobReader_t *) new(
            lfcSocketJobReader_class(),
            "lfcSocketJobReader", lfcSocketJob(), sizeof(lfcSocketJobReader_t),

            lfcObject_ctor, "ctor", public_lfcSocketJobReader_ctor,
            lfcObject_dtor, "dtor", public_lfcSocketJobReader_dtor,

            lfcSocketJob_doWork, "doWork", impl_lfcSocketJobReader__SocketJob__doWork,
            lfcSocketJob_reset, "reset", impl_lfcSocketJobReader__SocketJob__reset,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocketJobReader);

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
lfcSocketJobReader_t *lfcSocketJobReader_ctor (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    size_t len_to_read,
    fn_onReadComplete_cb onReadComplete
) {
    return lfcSocketJobReader_ctor_wRepeat(
        fd,
        context, ident,
        timeout_in_s,
        1,
        calloc(1, len_to_read), len_to_read, true,
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
lfcSocketJobReader_t *lfcSocketJobReader_ctor_wBuffer (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    char *read_buf,
    size_t len_to_read,
    fn_onReadComplete_cb onReadComplete
) {
    if (!read_buf) { return NULL; }
    if (!len_to_read) { return NULL; }

    return lfcSocketJobReader_ctor_wRepeat(
        fd,
        context, ident,
        timeout_in_s,
        1,
        read_buf, len_to_read, false,
        onReadComplete
    );
}

/**
 * Einen neuen Reader erstellen, der Lese-Buffer wird von uns alloziiert und dann im Callback zurueckgegeben.
 * Dieser Reader läuft immer und immer wieder und löäscht sich nicht nach einem Empfang.
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param timeout_in_s nach wieviel s soll das Lesen abgebrochen werden? (0 == kein timeout)
 * @param repeat Angabe, wie oft der Lese-Befehl ausgeführt werden soll (0 .. endlos, 1 .. one-shot, >1 .. Anzahl)
 * @param read_buf in diesen Buffer soll gelesen werden
 * @param len_to_read zu lesende Länge
 * qparam free_buf soll der uebergebene buffer im dtor gefreed werden?
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobReader_t *lfcSocketJobReader_ctor_wRepeat(
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    unsigned int repeat,
    char *read_buf,
    size_t len_to_read,
    bool free_buf,
    fn_onReadComplete_cb onReadComplete
) {
    if (!read_buf) { return NULL; }
    if (!len_to_read) { return NULL; }

    return (lfcSocketJobReader_t *)new(
        lfcSocketJobReader(),
        fd,
        context, ident,
        timeout_in_s,
        repeat,
        read_buf, len_to_read, free_buf,
        onReadComplete
    );
}
