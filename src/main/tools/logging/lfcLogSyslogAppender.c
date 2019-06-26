#include "lfcLogSyslogAppender.h"
#include "lfcLog.h"
#include "lfcLogFormatter.h"

#include <errno.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcLogSyslogAppender_t *_lfcLogSyslogAppender;
static const lfcLogSyslogAppender_class_t *_lfcLogSyslogAppender_class;
static lfcLogSyslogAppender_methods_t _lfcLogSyslogAppender_methods isAnUnused_param;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcLogSyslogAppender_t *public_lfcLogSyslogAppender_ctor (void *_self, va_list *app);
static lfcLogSyslogAppender_t *public_lfcLogSyslogAppender_dtor (lfcLogSyslogAppender_t *self);


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Constructor.
 */
static lfcLogSyslogAppender_t *public_lfcLogSyslogAppender_ctor(
    void *_self,
    va_list *app
) {
    lfcLogSyslogAppender_t *self = (lfcLogSyslogAppender_t *) lfcObject_super_ctor(
        lfcLogSyslogAppender(), _self
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
static lfcLogSyslogAppender_t *public_lfcLogSyslogAppender_dtor(
    lfcLogSyslogAppender_t *self
) {
    if (self->fmt) { free(self->fmt); }

    return lfcObject_super_dtor(lfcLogSyslogAppender(), self);
}

/**
 */
static int impl_lfcLogSyslogAppender__LogAppender__print (
    lfcLogAppender_t *_self,
    lfcLog_t *log
) {
    lfcLogSyslogAppender_t *self = asInstanceOf(lfcLogSyslogAppender(), _self);
    if (!self) return -EINVAL;

    char *out = lfcLogFormatter_formatAsString(self->fmt, log);
    if (out) {
        syslog(lfcLog_getLogLevel(log) | LOG_USER, "%s", out);

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
CLASS_CTOR__START(lfcLogSyslogAppender)
    CLASS_CTOR__INIT_SUPER(lfcLogSyslogAppender, lfcLogAppender)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcLogSyslogAppender_class_t *lfcLogSyslogAppender_class() {
    return _lfcLogSyslogAppender_class
        ? _lfcLogSyslogAppender_class
        : (_lfcLogSyslogAppender_class = (lfcLogSyslogAppender_class_t *) new(
            lfcLogAppender_class(), "lfcLogSyslogAppender_class", lfcLogAppender_class(), sizeof(lfcLogSyslogAppender_class_t),

            lfcObject_ctor, "", impl_lfcLogSyslogAppender_class_ctor,

            (void *) 0)
        );
}

const lfcLogSyslogAppender_t *lfcLogSyslogAppender() {
    return _lfcLogSyslogAppender
        ? _lfcLogSyslogAppender
        : (_lfcLogSyslogAppender = (lfcLogSyslogAppender_t *) new(
            lfcLogSyslogAppender_class(),
            "lfcLogSyslogAppender", lfcLogAppender(), sizeof(lfcLogSyslogAppender_t),

            lfcObject_ctor, "ctor", public_lfcLogSyslogAppender_ctor,
            lfcObject_dtor, "dtor", public_lfcLogSyslogAppender_dtor,

            lfcLogAppender_print, "print", impl_lfcLogSyslogAppender__LogAppender__print,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcLogSyslogAppender);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Einen neuen Appender erstellen
 */
lfcLogSyslogAppender_t *lfcLogSyslogAppender_ctor (
    const char *fmt
) {
    return (lfcLogSyslogAppender_t *)new(lfcLogSyslogAppender(), fmt);
}

lfcLogSyslogAppender_t *lfcLogSyslogAppender_ctor_stdFormat () {
    return lfcLogSyslogAppender_ctor("%D{%F %T} [%l] %p --- [%t->%T] --- %F:%f : %m");
}
