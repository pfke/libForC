#ifndef LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_

#include "../../core/lfcCore.h"
#include "../../core/lfcObject.h"
#include "lfcLogCommon.h"

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcOOP_defineClass(lfcLogAppender, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    int, print, (lfcLog_t *log)
)


#endif //LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_
