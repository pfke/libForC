#include "lfcLogInstance.h"

#include <stdlib.h>
#include "../core/lfcCore.h"


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static pthread_mutex_t lfcLogInstance_instance_mutex = PTHREAD_MUTEX_INITIALIZER;
static const  lfcLogInstance_t *lfcLogInstance_instance;

static const lfcLogInstance_t *_lfcLogInstance;
static const lfcLogInstance_class_t *_lfcLogInstance_class;
static lfcLogInstance_methods_t _lfcLogInstance_methods isAnUnused_param;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcLogInstance_t *public_lfcLogInstance_ctor (void *_self, va_list *app);
static lfcLogInstance_t *public_lfcLogInstance_dtor (lfcLogInstance_t *self);

static int public_lfcLogInstance_log (
    lfcLogInstance_t *self,
    lfcLogInstance_loglevel_e log_level,
    const char *prefix,
    const char *format,
    va_list *args
);
static int public_lfcLogInstance_redirectToStderr(lfcLogInstance_t *self);
static int public_lfcLogInstance_redirectToSyslog(lfcLogInstance_t *self);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

static const char *static_lfcLogInstance_tostr (
    lfcLogInstance_loglevel_e log_level
) isAnUnused_func;
static const char *static_lfcLogInstance_tostr (
    lfcLogInstance_loglevel_e log_level
) {
    switch (log_level) {
        case LOGLEVEL_EMERG:    return ("UR");
        case LOGLEVEL_ALERT:    return ("AL");
        case LOGLEVEL_CRIT:     return ("CR");
        case LOGLEVEL_ERR:      return ("EE");
        case LOGLEVEL_WARNING:  return ("WW");
        case LOGLEVEL_NOTICE:   return ("NN");
        case LOGLEVEL_INFO:     return ("II");
        case LOGLEVEL_DEBUG:    return ("DD");

        default: return ("??");
    }
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcLogInstance_t *public_lfcLogInstance_ctor (
    void *_self,
    va_list *app
) {
    lfcLogInstance_t *self = (lfcLogInstance_t *) lfcObject_super_ctor(lfcLogInstance(), _self);

    ASSERT_PTR(self, err_self, "could not create instance");

    // read args
    self->current_log_level = va_arg(*app, lfcLogInstance_loglevel_e);

    self->redirectTo = LOG_REDIRECTTO_SYSLOG;

    return self;

err_self:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLogInstance_t *public_lfcLogInstance_dtor (
    lfcLogInstance_t *self
) {
    lfcObject_super_dtor(lfcLogInstance(), self);

    return self;
}

static int public_lfcLogInstance_log (
    lfcLogInstance_t *self,
    lfcLogInstance_loglevel_e log_level,
    const char *prefix,
    const char *format,
    va_list *args
) {
    char out_buffer [512];
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [20];

    // soll geloggt werden?
    if (log_level > self->current_log_level) {
        return 0;
    }

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (timeinfo) {
        strftime(buffer, sizeof(buffer), "%F@%T", timeinfo); // YYYY-MM-DD@HH:MM:SS
    } else {
        snprintf(buffer, sizeof(buffer), "no-valid-time");
    }

    memset(out_buffer, 0, sizeof(out_buffer));
    snprintf(out_buffer, sizeof(out_buffer), "%s {%s} [%s]: ", buffer, prefix, static_lfcLogInstance_tostr(log_level));

    size_t offs = 29 + strlen(prefix);
    vsnprintf(&out_buffer[offs], sizeof(out_buffer) - offs, format, *args);

    switch (self->redirectTo) {
        case LOG_REDIRECTTO_STDERR:
            fprintf(stderr, "%s\n", out_buffer);
            break;

        case LOG_REDIRECTTO_SYSLOG:
            syslog(log_level | LOG_USER, "%s", out_buffer);
            break;

        default:
            break;
    }

    return 0;
}

static int public_lfcLogInstance_redirectToStderr (
    lfcLogInstance_t *self
) {
    self->redirectTo = LOG_REDIRECTTO_STDERR;

    return 0;
}

static int public_lfcLogInstance_redirectToSyslog (
    lfcLogInstance_t *self
) {
    self->redirectTo = LOG_REDIRECTTO_SYSLOG;

    return 0;
}

static int public_lfcLogInstance_setDebugLevel (
    lfcLogInstance_t *self,
    lfcLogInstance_loglevel_e level
) {
    self->current_log_level = level;

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
CLASS_CTOR__START(lfcLogInstance)
        OVERRIDE_METHOD(lfcLogInstance, log)
        OVERRIDE_METHOD(lfcLogInstance, redirectToStderr)
        OVERRIDE_METHOD(lfcLogInstance, redirectToSyslog)
        OVERRIDE_METHOD(lfcLogInstance, setDebugLevel)
    CLASS_CTOR__INIT_SUPER(lfcLogInstance, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcLogInstance_class_t *lfcLogInstance_class() {
    return _lfcLogInstance_class
           ? _lfcLogInstance_class
           : (_lfcLogInstance_class = (lfcLogInstance_class_t *) new(
            lfcObject_class(), "lfcLogInstance_class", lfcObject_class(), sizeof(lfcLogInstance_class_t),

            lfcObject_ctor, "", impl_lfcLogInstance_class_ctor,

            (void *) 0)
           );
}

const lfcLogInstance_t *lfcLogInstance() {
    return _lfcLogInstance
           ? _lfcLogInstance
           : (_lfcLogInstance = (lfcLogInstance_t *) new(
            lfcLogInstance_class(),
            "lfcLogInstance", lfcObject(), sizeof(lfcLogInstance_t),

            lfcObject_ctor, "ctor", public_lfcLogInstance_ctor,
            lfcObject_dtor, "dtor", public_lfcLogInstance_dtor,

            lfcLogInstance_log, "log", public_lfcLogInstance_log,
            lfcLogInstance_redirectToStderr, "redirectToStderr", public_lfcLogInstance_redirectToStderr,
            lfcLogInstance_redirectToSyslog, "redirectToSyslog", public_lfcLogInstance_redirectToSyslog,
            lfcLogInstance_setDebugLevel, "setDebugLevel", public_lfcLogInstance_setDebugLevel,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcLogInstance);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_accessor(lfcLogInstance, log, int, lfcLogInstance_loglevel_e, const char *, const char *, va_list *);
lfcOOP_accessor(lfcLogInstance, redirectToStderr, int);
lfcOOP_accessor(lfcLogInstance, redirectToSyslog, int);
lfcOOP_accessor(lfcLogInstance, setDebugLevel, int, lfcLogInstance_loglevel_e);

/**
 * Erzeugt eine lfcLogInstance Instanz.
 */
const lfcLogInstance_t *lfcLogInstance_singleton () {
        // instance bereits angelegt?
    if (!lfcLogInstance_instance) {
        // geschützt eine instance anlegen
        if (!pthread_mutex_lock(&lfcLogInstance_instance_mutex)) {
            if (!lfcLogInstance_instance) {
                lfcLogInstance_instance = (lfcLogInstance_t *)new(lfcLogInstance(), LOGLEVEL_WARNING);
            }

            pthread_mutex_unlock(&lfcLogInstance_instance_mutex);
        }
    }

    return lfcLogInstance_instance;
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
