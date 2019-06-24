#include "lfcLogger.h"

#include <stdlib.h>
#include "../core/lfcCore.h"


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcLogger_t *_lfcLogger;
static const lfcLogger_class_t *_lfcLogger_class;
static lfcLogger_methods_t _lfcLogger_methods isAnUnused_param;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcLogger_t *public_lfcLogger_ctor (void *_self, va_list *app);
static lfcLogger_t *public_lfcLogger_dtor (lfcLogger_t *self);

static int public_lfcLogger_log_EMERG_va    (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_ALERT_va    (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_CRIT_va     (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_ERR_va      (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_WARNING_va  (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_NOTICE_va   (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_INFO_va     (lfcLogger_t *self, const char *format, va_list *args);
static int public_lfcLogger_log_DEBUG_va    (lfcLogger_t *self, const char *format, va_list *args);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcLogger_t *public_lfcLogger_ctor (
    void *_self,
    va_list *app
) {
    lfcLogger_t *self = (lfcLogger_t *) lfcObject_super_ctor(lfcLogger(), _self);

    ASSERT_PTR(self, err_self, "could not create instance");

    // read args
    self->logInstance = va_arg(*app, lfcLogInstance_t *);
    self->prefix = va_arg(*app, char *);

    return self;

err_self:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLogger_t *public_lfcLogger_dtor (
    lfcLogger_t *self
) {
    lfcObject_super_dtor(lfcLogger(), self);

    if (self->prefix) free(self->prefix);

    return self;
}

static int public_lfcLogger_log_EMERG_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_EMERG, self->prefix, format, args);
}

static int public_lfcLogger_log_ALERT_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_ALERT, self->prefix, format, args);
}

static int public_lfcLogger_log_CRIT_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_CRIT, self->prefix, format, args);
}

static int public_lfcLogger_log_ERR_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_ERR, self->prefix, format, args);
}

static int public_lfcLogger_log_WARNING_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_WARNING, self->prefix, format, args);
}

static int public_lfcLogger_log_NOTICE_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_NOTICE, self->prefix, format, args);
}

static int public_lfcLogger_log_INFO_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_INFO, self->prefix, format, args);
}

static int public_lfcLogger_log_DEBUG_va (
    lfcLogger_t *self,
    const char *format,
    va_list *args
) {
    return lfcLogInstance_log(self->logInstance, LOGLEVEL_DEBUG, self->prefix, format, args);
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
CLASS_CTOR__START(lfcLogger)
        OVERRIDE_METHOD(lfcLogger, log_EMERG_va)
        OVERRIDE_METHOD(lfcLogger, log_ALERT_va)
        OVERRIDE_METHOD(lfcLogger, log_CRIT_va)
        OVERRIDE_METHOD(lfcLogger, log_ERR_va)
        OVERRIDE_METHOD(lfcLogger, log_WARNING_va)
        OVERRIDE_METHOD(lfcLogger, log_NOTICE_va)
        OVERRIDE_METHOD(lfcLogger, log_INFO_va)
        OVERRIDE_METHOD(lfcLogger, log_DEBUG_va)
    CLASS_CTOR__INIT_SUPER(lfcLogger, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcLogger_class_t *lfcLogger_class() {
    return _lfcLogger_class
           ? _lfcLogger_class
           : (_lfcLogger_class = (lfcLogger_class_t *) new(
            lfcObject_class(), "lfcLogger_class", lfcObject_class(), sizeof(lfcLogger_class_t),

            lfcObject_ctor, "", impl_lfcLogger_class_ctor,

            (void *) 0)
           );
}

const lfcLogger_t *lfcLogger() {
    return _lfcLogger
           ? _lfcLogger
           : (_lfcLogger = (lfcLogger_t *) new(
            lfcLogger_class(),
            "lfcLogger", lfcObject(), sizeof(lfcLogger_t),

            lfcObject_ctor, "ctor", public_lfcLogger_ctor,
            lfcObject_dtor, "dtor", public_lfcLogger_dtor,

            lfcLogger_log_EMERG_va, "log_EMERG_va", public_lfcLogger_log_EMERG_va,
            lfcLogger_log_ALERT_va, "log_ALERT_va", public_lfcLogger_log_ALERT_va,
            lfcLogger_log_CRIT_va, "log_CRIT_va", public_lfcLogger_log_CRIT_va,
            lfcLogger_log_ERR_va, "log_ERR_va", public_lfcLogger_log_ERR_va,
            lfcLogger_log_WARNING_va, "log_WARNING_va", public_lfcLogger_log_WARNING_va,
            lfcLogger_log_NOTICE_va, "log_NOTICE_va", public_lfcLogger_log_NOTICE_va,
            lfcLogger_log_INFO_va, "log_INFO_va", public_lfcLogger_log_INFO_va,
            lfcLogger_log_DEBUG_va, "log_DEBUG_va", public_lfcLogger_log_DEBUG_va,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcLogger);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

IMPL_API__wRET__w_2PARAM (lfcLogger, log_EMERG_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_ALERT_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_CRIT_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_ERR_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_WARNING_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_NOTICE_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_INFO_va, int, const char *, va_list *);
IMPL_API__wRET__w_2PARAM (lfcLogger, log_DEBUG_va, int, const char *, va_list *);

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLogger_t *lfcLogger_ctor (
    const char *format, ...
) {
    char buf[100];
    va_list args;

    va_start (args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end (args);

    return (lfcLogger_t *)new(lfcLogger(), lfcLogInstance_singleton(), strdup(buf));
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

void lfcLogger_log_EMERG (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_EMERG_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_ALERT (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_ALERT_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_CRIT (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_CRIT_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_ERR (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_ERR_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_WARNING (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_WARNING_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_NOTICE (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_NOTICE_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_INFO (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_INFO_va(self, format, &args);
    va_end (args);
}

void lfcLogger_log_DEBUG (lfcLogger_t *self, const char *format, ...) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_DEBUG_va(self, format, &args);
    va_end (args);
}
