#ifndef LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_

#include "../../core/lfcObject.h"
#include "lfcLogAppender.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLogConsoleAppender)

struct lfcLogConsoleAppender { const struct lfcLogAppender _;
    char *fmt;
};

struct lfcLogConsoleAppender_class { const struct lfcLogAppender_class _;
    method_t getFmt;
};

struct lfcLogConsoleAppender_methods {
    int (*getFmt)(lfcLogConsoleAppender_t *self);

    // super
    const lfcLogAppender_methods_t *base;
};

/**
 * Erstellung einer Instanz und Uebergabe des Formattes, welches folgenden Aufbau haben kann:
 *
 *
 */
lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor(
    const char *fmt
);

/**
 * Std-Format: "%c%D{%F %T} [%l] %p --- [%t->%T] --- %F:%f : %m%C"
 *
 * ergibt:
 *   2019-06-26 10:24:55 [UR] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:29 : bin hier: achso
 *   2019-06-26 10:24:55 [AL] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:30 : bin hier: achso
 *   2019-06-26 10:24:55 [CR] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:31 : bin hier: achso
 *   2019-06-26 10:24:55 [EE] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:32 : err 5
 *   2019-06-26 10:24:55 [WW] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:33 : debug 1
 *   2019-06-26 10:24:55 [NN] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:34 : debug 1
 *   2019-06-26 10:24:55 [II] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:35 : debug 1
 *   2019-06-26 10:24:55 [DD] dumi_13 --- [4160170752->libForC_runner] --- runner_fn:36 : debug 2
 */
lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor_stdFormat (void);

int lfcLogConsoleAppender_getFmt(lfcLogConsoleAppender_t *self);

#endif //LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_
