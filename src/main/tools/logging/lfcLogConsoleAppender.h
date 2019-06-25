#ifndef LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_

#include "../../core/lfcObject.h"
#include "lfcLogAppender.h"

#define DEFAULT_LIST_INITIAL_SIZE         10
#define DEFAULT_LIST_INCREASE_BY          10

DEFINE_CLASS(lfcLogConsoleAppender)

struct lfcLogConsoleAppender { const struct lfcObject _;
    char *fmt;
};

struct lfcLogConsoleAppender_class { const struct lfcObject_class _;
    lfcLogAppender_t lfcLogAppender;
};

struct lfcLogConsoleAppender_methods {
    // super
    const lfcObject_methods_t *base;

    // ifaces
    const lfcLogAppender_methods_t *lfcLogAppender;
};

/**
 * Erstellung einer Instanz und Uebergabe des Formattes, welches folgenden Aufbau haben kann:
 *
 *
 */
lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor(
    const char *fmt
);

#endif //LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_
