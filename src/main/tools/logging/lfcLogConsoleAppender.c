#include "lfcLogConsoleAppender.h"

#include <errno.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcLogConsoleAppender_t *_lfcLogConsoleAppender;
static const lfcLogConsoleAppender_class_t *_lfcLogConsoleAppender_class;
static lfcLogConsoleAppender_methods_t _lfcLogConsoleAppender_methods;

/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/

static int impl_lfcLogConsoleAppender__LogAppender__put(lfcLogAppender_t *_self, lfcLog_t *log);


/**
 * Constructor selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcLogConsoleAppender_t *public_lfcLogConsoleAppender_ctor(
    void *_self,
    va_list *app
) {
    lfcLogConsoleAppender_t *self = (lfcLogConsoleAppender_t *) lfcObject_super_ctor(lfcLogConsoleAppender(), _self);

    if (self == 0)
        return 0;

    // read args
    const char *fmt = va_arg(*app, const char *);
    if (!fmt) { goto err; }
    self->fmt = strdup(fmt);

    return self;

err:
    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcLogConsoleAppender_t *public_lfcLogConsoleAppender_dtor(
    lfcLogConsoleAppender_t *self
) {
    if (self->fmt) { free(self->fmt); }

    return lfcObject_super_dtor(lfcLogConsoleAppender(), self);
}

/**
 */
static int impl_lfcLogConsoleAppender__LogAppender__put(
    lfcLogAppender_t *_self,
    lfcLog_t *log
) {
    lfcLogConsoleAppender_t *self = asInstanceOf(lfcLogConsoleAppender(), _self);

    if (!self) return -EINVAL;

//    lfcLog_format(log);

    fprintf(stdout, "%s\n", "k");

    return 0;
}

/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcLogConsoleAppender)
    CLASS_CTOR__INIT_SUPER(lfcLogConsoleAppender, lfcObject)
    CLASS_CTOR__INIT_IFACES()
        CLASS_IMPLEMENT_IFACE(lfcLogConsoleAppender, lfcLogAppender)
CLASS_CTOR__END()

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
const lfcLogConsoleAppender_class_t *lfcLogConsoleAppender_class() {
    return _lfcLogConsoleAppender_class
           ? _lfcLogConsoleAppender_class
           : (_lfcLogConsoleAppender_class = (lfcLogConsoleAppender_class_t *) new(
            lfcObject_class(), "lfcLogConsoleAppender_class", lfcObject_class(), sizeof(lfcLogConsoleAppender_class_t),

            lfcObject_ctor, "", impl_lfcLogConsoleAppender_class_ctor,

            (void *) 0)
           );
}

const lfcLogConsoleAppender_t *lfcLogConsoleAppender() {
    return _lfcLogConsoleAppender
           ? _lfcLogConsoleAppender
           : (_lfcLogConsoleAppender = (lfcLogConsoleAppender_t *) new(
            lfcLogConsoleAppender_class(),
            "lfcLogConsoleAppender", lfcObject(), sizeof(lfcLogConsoleAppender_t),

            lfcObject_ctor, "ctor", public_lfcLogConsoleAppender_ctor,
            lfcObject_dtor, "dtor", public_lfcLogConsoleAppender_dtor,

            lfcLogAppender_put, "put", impl_lfcLogConsoleAppender__LogAppender__put,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcLogConsoleAppender);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Liste mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor(
    const char *fmt
) {
    return (lfcLogConsoleAppender_t *)new(lfcLogConsoleAppender(), fmt);
}
