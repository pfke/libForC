#ifndef LIBFORC_TOOLS_LOGGER_H_
#define LIBFORC_TOOLS_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lfcLogInstance.h"
#include "../core/lfcObject.h"
#include <pthread.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLogger);

#define log_ENABLE_EMERG
#define log_ENABLE_ALERT
#define log_ENABLE_CRIT
#define log_ENABLE_ERR
#define log_ENABLE_WARNING
#define log_ENABLE_NOTICE
#define log_ENABLE_INFO
#define log_ENABLE_DEBUG

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcLogger { const struct lfcObject _;
    lfcLogInstance_t *logInstance;
    char *prefix;

    /**
     * Wird genutzt die read-Funktion so lange zu blocken, bis alles geschrieben werden konnte.
     */
    pthread_mutex_t write_mutex;
};

struct lfcLogger_class { const struct lfcObject_class _;
    method_t log_EMERG_va;
    method_t log_ALERT_va;
    method_t log_CRIT_va;
    method_t log_ERR_va;
    method_t log_WARNING_va;
    method_t log_NOTICE_va;
    method_t log_INFO_va;
    method_t log_DEBUG_va;
};

struct lfcLogger_methods {
    int (*log_EMERG_va)     (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_ALERT_va)     (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_CRIT_va)      (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_ERR_va)       (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_WARNING_va)   (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_NOTICE_va)    (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_INFO_va)      (lfcLogger_t *self, const char *format, va_list *args);
    int (*log_DEBUG_va)     (lfcLogger_t *self, const char *format, va_list *args);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLogger_t *lfcLogger_ctor(const char *format, ...);

int lfcLogger_log_EMERG_va      (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_ALERT_va      (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_CRIT_va       (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_ERR_va        (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_WARNING_va    (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_NOTICE_va     (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_INFO_va       (lfcLogger_t *self, const char *format, va_list *args);
int lfcLogger_log_DEBUG_va      (lfcLogger_t *self, const char *format, va_list *args);

void lfcLogger_log_EMERG         (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_ALERT         (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_CRIT          (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_ERR           (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_WARNING       (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_NOTICE        (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_INFO          (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_DEBUG         (lfcLogger_t *self, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGER_H_ ---*/
