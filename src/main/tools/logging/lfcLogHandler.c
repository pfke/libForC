#include "lfcLogHandler.h"

#include <stdlib.h>
#include <errno.h>


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcLogHandler, lfcObject,
    int, addAppender, (lfcLogAppender_t *),
    int, log,         (lfcLog_t *),
    int, setLogLevel, (lfcLogging_loglevel_e)
)


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static pthread_mutex_t lfcLogHandler_instance_mutex = PTHREAD_MUTEX_INITIALIZER;
static const  lfcLogHandler_t *lfcLogHandler_instance;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

static int private_lfcLogHandler_log (
    lfcLogHandler_t *self,
    lfcLog_t *log
) {
    if (!log) { return -EINVAL; }

    lfcIIterable_foreach(self->appenders, lambda_void((void *item) {
        lfcLogAppender_print(item, log);
    }));

    delete(log);

//    switch (self->redirectTo) {
//        case LOG_REDIRECTTO_STDERR:
//            fprintf(stderr, "%s\n", buf);
//            break;
//
//        case LOG_REDIRECTTO_SYSLOG:
//            syslog(log_level | LOG_USER, "%s", buf);
//            break;
//
//        default:
//            break;
//    }
//
//    free(buf);

    return 0;
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcLogHandler_t *public_lfcLogHandler_ctor (
    void *_self,
    va_list *app
) {
    lfcLogHandler_t *self = (lfcLogHandler_t *) lfcObject_super_ctor(lfcLogHandler(), _self);

    ASSERT_PTR(self, err_self, "could not create instance");

    // read args
    self->current_log_level = va_arg(*app, lfcLogging_loglevel_e);
    self->appenders = lfcList_ctor();
    if (!self->appenders) { goto err_self; }

    return self;

err_self:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLogHandler_t *public_lfcLogHandler_dtor (
    lfcLogHandler_t *self
) {
    delete(self->appenders);

    return lfcObject_super_dtor(lfcLogHandler(), self);
}

static int public_lfcLogHandler_addAppender (
    lfcLogHandler_t *self,
    lfcLogAppender_t *appender
) {
    if (!appender) { return -EINVAL; }

    return lfcList_add(self->appenders, appender);
}

static int public_lfcLogHandler_log (
    lfcLogHandler_t *self,
    lfcLog_t *log
) {
    // soll geloggt werden?
    if (self->current_log_level < lfcLog_getLogLevel((lfcLog_t *)log)) {
        return 0;
    }

    return private_lfcLogHandler_log(self, log);
}

static int public_lfcLogHandler_setLogLevel (
    lfcLogHandler_t *self,
    lfcLogging_loglevel_e level
) {
    self->current_log_level = level;

    return 0;
}


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Erzeugt eine lfcLogHandler Instanz.
 */
lfcLogHandler_t *lfcLogHandler_ctor () {
    return (lfcLogHandler_t *)new(lfcLogHandler(), LOGLEVEL_WARNING);
}

/**
 * Erzeugt eine lfcLogHandler Instanz.
 */
const lfcLogHandler_t *lfcLogHandler_singleton () {
    // instance bereits angelegt?
    if (!lfcLogHandler_instance) {
        // geschützt eine instance anlegen
        if (!pthread_mutex_lock(&lfcLogHandler_instance_mutex)) {
            if (!lfcLogHandler_instance) {
                lfcLogHandler_instance = lfcLogHandler_ctor();
            }

            pthread_mutex_unlock(&lfcLogHandler_instance_mutex);
        }
    }

    return lfcLogHandler_instance;
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
