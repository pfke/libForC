#ifndef LIBFORC_TOOLS_LOGGING_LOGSYSLOGAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGSYSLOGAPPENDER_H_

#include "../../core/lfcObject.h"
#include "lfcLogAppender.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcOOP_defineClass(lfcLogSyslogAppender, lfcLogAppender,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    char *fmt;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    int, getFmt, ()
)

/**
 * Erstellung einer Instanz und Uebergabe des Formattes, welches folgenden Aufbau haben kann:
 *
 *
 */
lfcLogSyslogAppender_t *lfcLogSyslogAppender_ctor(
    const char *fmt
);

/**
 * Std-Format: "%D{%F %T} [%l] %p --- [%t->%T] --- %F:%f : %m"
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
lfcLogSyslogAppender_t *lfcLogSyslogAppender_ctor_stdFormat ();

#endif //LIBFORC_TOOLS_LOGGING_LOGSYSLOGAPPENDER_H_
