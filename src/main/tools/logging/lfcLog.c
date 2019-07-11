#include "lfcLog.h"

#include <stdlib.h>
#include <sys/prctl.h>


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcLog, lfcObject,
    const char *,          getLoggerPrefix, (),
    lfcLogging_loglevel_e, getLogLevel, (),
    const char *,          getMessage, (),
    const char *,          getMethod, (),
    int,                   getMethodLine, (),
    time_t,                getTimeStamp, (),
    pthread_t,             getThreadId, (),
    const char *,          getThreadName, ()
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
    self->method = va_arg(*app, const char *);
    self->methodLine = va_arg(*app, int);
    self->threadId = va_arg(*app, pthread_t);
    const char *threadName = va_arg(*app, const char *);
    const char *message = va_arg(*app, const char *);

    if (!loggerPrefix) { goto err; }
    self->loggerPrefix = strdup(loggerPrefix);

    if (!threadName) { goto err; }
    self->threadName = strdup(threadName);

    if (!message) { goto err; }
    self->message = strdup(message);


    return self;

err:
    if (self && self->loggerPrefix) { free(self->loggerPrefix); }
    if (self && self->threadName) { free(self->threadName); }
    if (self && self->message) { free(self->message); }

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLog_t *public_lfcLog_dtor (
    lfcLog_t *self
) {
    if (self->loggerPrefix) { free(self->loggerPrefix); }
    if (self->message) { free(self->message); }
    if (self->threadName) { free(self->threadName); }

    return lfcObject_super_dtor(lfcLog(), self);
}

static lfcLogging_loglevel_e public_lfcLog_getLogLevel    (lfcLog_t *self) { return self->logLevel; }
static const char *          public_lfcLog_getLoggerPrefix(lfcLog_t *self) { return self->loggerPrefix; }
static const char *          public_lfcLog_getMessage     (lfcLog_t *self) { return self->message; }
static const char *          public_lfcLog_getMethod      (lfcLog_t *self) { return self->method; }
static int                   public_lfcLog_getMethodLine  (lfcLog_t *self) { return self->methodLine; }
static time_t                public_lfcLog_getTimeStamp   (lfcLog_t *self) { return self->timeStamp; }
static pthread_t             public_lfcLog_getThreadId    (lfcLog_t *self) { return self->threadId; }
static const char *          public_lfcLog_getThreadName  (lfcLog_t *self) { return self->threadName; }


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Erzeugt eine lfcLog Instanz.
 */
lfcLog_t *lfcLog_ctor (
    time_t timeStamp,
    lfcLogging_loglevel_e logLevel,
    const char *loggerPrefix,
    const char *method,
    int methodLine,
    pthread_t threadId,
    const char *message
) {
    char threadName[100];

    memset(threadName, 0, sizeof(threadName));
    prctl(PR_GET_NAME, threadName, 0,0,0);

    return (lfcLog_t *)new(lfcLog(),
        timeStamp,
        logLevel,
        loggerPrefix,
        method,
        methodLine,
        threadId,
        threadName,
        message
    );
}
