#include "lfcLog.h"

#include <stdlib.h>


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcLog, lfcObject,
    lfcLogging_loglevel_e, getLogLevel, ()
)

/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcLog_t *public_lfcLog_ctor (
    void *_self,
    va_list *app
) {
    lfcLog_t *self = (lfcLog_t *) lfcObject_super_ctor(lfcLog(), _self);

    ASSERT_PTR(self, err, "could not create instance");

    // read args
    self->timeStamp = va_arg(*app, time_t);
    self->logLevel = va_arg(*app, lfcLogging_loglevel_e);

    const char *loggerPrefix = va_arg(*app, const char *);
    if (!loggerPrefix) { goto err; }
    self->loggerPrefix = strdup(loggerPrefix);

    const char *txt = va_arg(*app, const char *);
    if (!txt) { goto err; }
    self->txt = strdup(txt);

    return self;

err:
    if (self->loggerPrefix) { free(self->loggerPrefix); }
    if (self->txt) { free(self->txt); }

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLog_t *public_lfcLog_dtor (
    lfcLog_t *self
) {
    if (self->loggerPrefix) { free(self->loggerPrefix); }
    if (self->txt) { free(self->txt); }

    return lfcObject_super_dtor(lfcLog(), self);
}

static lfcLogging_loglevel_e public_lfcLog_getLogLevel (
    lfcLog_t *self
) {
    return self->logLevel;
}

/**
 * Erzeugt eine lfcLog Instanz.
 */
lfcLog_t *lfcLog_ctor (
    time_t timeStamp,
    lfcLogging_loglevel_e logLevel,
    const char *logger_prefix,
    const char *txt
) {
    return (lfcLog_t *)new(lfcLog(), timeStamp, logLevel, logger_prefix, txt);
}
