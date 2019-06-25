#ifndef LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_

#include "../../core/lfcObject.h"
#include "lfcLogAppender.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcOOP_defineClass(lfcLogConsoleAppender, lfcLogAppender,
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
lfcLogConsoleAppender_t *lfcLogConsoleAppender_ctor(
    const char *fmt
);

#endif //LIBFORC_TOOLS_LOGGING_LOGCONSOLEAPPENDER_H_
