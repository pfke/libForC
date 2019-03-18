#include "lfcSocketJobAcceptConn.h"

#include <stdlib.h>
#include <asm-generic/errno-base.h>
#include "../core/lfcCore.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocketJobAcceptConn_t *_lfcSocketJobAcceptConn;
static const lfcSocketJobAcceptConn_class_t *_lfcSocketJobAcceptConn_class;
static lfcSocketJobAcceptConn_methods_t _lfcSocketJobAcceptConn_methods isAnUnused_param;

char local_unused_buf;

/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcSocketJobAcceptConn_t *public_lfcSocketJobAcceptConn_ctor (void *_self, va_list *app);
static lfcSocketJobAcceptConn_t *public_lfcSocketJobAcceptConn_dtor (lfcSocketJobAcceptConn_t *self);

static int impl_lfcSocketJobAcceptConn__SocketJob__doWork (lfcSocketJob_t *_self);

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
static lfcSocketJobAcceptConn_t *public_lfcSocketJobAcceptConn_ctor(
    void *_self,
    va_list *app
) {
    int fd = va_arg(*app, int);
    void *context = va_arg(*app, void *);
    void *ident = va_arg(*app, void *);
    fn_onReadComplete_cb onReadComplete = va_arg(*app, fn_onReadComplete_cb);

    lfcSocketJobAcceptConn_t *self = (lfcSocketJobAcceptConn_t *) lfcObject_super_ctor(
        lfcSocketJobAcceptConn(), _self,
        fd, context, ident, 0, 0,
        &local_unused_buf, sizeof(local_unused_buf), onReadComplete
    );
    ASSERT_PTR(self, err_self, "could not instantiate super");

    return self;

err_self:
    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcSocketJobAcceptConn_t *public_lfcSocketJobAcceptConn_dtor(
    lfcSocketJobAcceptConn_t *self
) {
    lfcObject_super_dtor(lfcSocketJobAcceptConn(), self);

    return self;
}

/**
 * Socket ist bereit -> lesen.
 */
static int impl_lfcSocketJobAcceptConn__SocketJob__doWork (
    lfcSocketJob_t *_self
) {
    lfcSocketJobAcceptConn_t *self = asInstanceOf(lfcSocketJobAcceptConn(), _self);
    if (!self) return -EINVAL;
    if (lfcSocketJob_isEnabled(_self) == false) return 1;

    self->_.onReadComplete(self->_._.context, self->_._.ident, 0, NULL);

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
CLASS_CTOR__START(lfcSocketJobAcceptConn)
    CLASS_CTOR__INIT_SUPER(lfcSocketJobAcceptConn, lfcSocketJobReader)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocketJobAcceptConn_class_t *lfcSocketJobAcceptConn_class() {
    return _lfcSocketJobAcceptConn_class
           ? _lfcSocketJobAcceptConn_class
           : (_lfcSocketJobAcceptConn_class = (lfcSocketJobAcceptConn_class_t *) new(
            lfcSocketJobReader_class(), "lfcSocketJobAcceptConn_class", lfcSocketJobReader_class(), sizeof(lfcSocketJobAcceptConn_class_t),

            lfcObject_ctor, "", impl_lfcSocketJobAcceptConn_class_ctor,

            (void *) 0)
           );
}

const lfcSocketJobAcceptConn_t *lfcSocketJobAcceptConn() {
    return _lfcSocketJobAcceptConn
           ? _lfcSocketJobAcceptConn
           : (_lfcSocketJobAcceptConn = (lfcSocketJobAcceptConn_t *) new(
            lfcSocketJobAcceptConn_class(),
            "lfcSocketJobAcceptConn", lfcSocketJobReader(), sizeof(lfcSocketJobAcceptConn_t),

            lfcObject_ctor, "ctor", public_lfcSocketJobAcceptConn_ctor,
            lfcObject_dtor, "dtor", public_lfcSocketJobAcceptConn_dtor,

            lfcSocketJob_doWork, "doWork", impl_lfcSocketJobAcceptConn__SocketJob__doWork,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocketJobAcceptConn);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Einen neuen Reader erstellen
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobAcceptConn_t *lfcSocketJobAcceptConn_ctor (
    int fd,
    void *context,
    void *ident,
    fn_onReadComplete_cb onReadComplete
) {
    return (lfcSocketJobAcceptConn_t *)new(
        lfcSocketJobAcceptConn(),
        fd,
        context, ident,
        onReadComplete
    );
}
