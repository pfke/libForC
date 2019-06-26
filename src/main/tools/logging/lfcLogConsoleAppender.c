#include "lfcLogConsoleAppender.h"
#include "lfcLog.h"
#include "lfcLogFormatter.h"

#include <errno.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcLogConsoleAppender_t *_lfcLogConsoleAppender;
static const lfcLogConsoleAppender_class_t *_lfcLogConsoleAppender_class;
static lfcLogConsoleAppender_methods_t _lfcLogConsoleAppender_methods isAnUnused_param;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcLogConsoleAppender_t *public_lfcLogConsoleAppender_ctor (void *_self, va_list *app);
static lfcLogConsoleAppender_t *public_lfcLogConsoleAppender_dtor (lfcLogConsoleAppender_t *self);


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Constructor.
 */
static lfcLogConsoleAppender_t *public_lfcLogConsoleAppender_ctor(
    void *_self,
    va_list *app
) {
    lfcLogConsoleAppender_t *self = (lfcLogConsoleAppender_t *) lfcObject_super_ctor(
        lfcLogConsoleAppender(), _self
    );
    ASSERT_PTR(self, err, "could not instantiate super");

    // read args
    const char *fmt =  va_arg(*app, char *);
    if (!fmt) { goto err; }
    self->fmt = strdup(fmt);

    return self;

err:
    if (self->fmt) { free(self->fmt); }

    return NULL;
}

/**
 * Destructor.
 */
static lfcLogConsoleAppender_t *public_lfcLogConsoleAppender_dtor(
    lfcLogConsoleAppender_t *self
) {
    if (self->fmt) { free(self->fmt); }

    return lfcObject_super_dtor(lfcLogConsoleAppender(), self);
}

/**
 */
static int impl_lfcLogConsoleAppender__LogAppender__print (
    lfcLogAppender_t *_self,
    lfcLog_t *log
) {
    lfcLogConsoleAppender_t *self = asInstanceOf(lfcLogConsoleAppender(), _self);
    if (!self) return -EINVAL;

    char *out = lfcLogFormatter_formatAsString(self->fmt, log);
    if (out) {
        fprintf(stdout, "%s\n", out);

        free(out);
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
CLASS_CTOR__START(lfcLogConsoleAppender)
    CLASS_CTOR__INIT_SUPER(lfcLogConsoleAppender, lfcLogAppender)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcLogConsoleAppender_class_t *lfcLogConsoleAppender_class() {
    return _lfcLogConsoleAppender_class
        ? _lfcLogConsoleAppender_class
        : (_lfcLogConsoleAppender_class = (lfcLogConsoleAppender_class_t *) new(
            lfcLogAppender_class(), "lfcLogConsoleAppender_class", lfcLogAppender_class(), sizeof(lfcLogConsoleAppender_class_t),

            lfcObject_ctor, "", impl_lfcLogConsoleAppender_class_ctor,

            (void *) 0)
        );
}

const lfcLogConsoleAppender_t *lfcLogConsoleAppender() {
    return _lfcLogConsoleAppender
        ? _lfcLogConsoleAppender
        : (_lfcLogConsoleAppender = (lfcLogConsoleAppender_t *) new(
            lfcLogConsoleAppender_class(),
            "lfcLogConsoleAppender", lfcLogAppender(), sizeof(lfcLogConsoleAppender_t),

            lfcObject_ctor, "ctor", public_lfcLogConsoleAppender_ctor,
            lfcObject_dtor, "dtor", public_lfcLogConsoleAppender_dtor,

            lfcLogAppender_print, "print", impl_lfcLogConsoleAppender__LogAppender__print,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcLogConsoleAppender);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Einen neuen Appender erstellen
 */
lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor (
    const char *fmt
) {
    return (lfcLogConsoleAppender_t *)new(lfcLogConsoleAppender(), fmt);
}

lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor_stdFormat () {
    return lfcLogConsoleAppender_ctor("%c%D{%F %T} [%l] %p --- [%t->%T] --- %F:%f : %m%C");
}
