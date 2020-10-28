#ifndef LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_

#include "../../core/lfcCore.h"
#include "../../core/lfcObject.h"
#include "lfcLogCommon.h"

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLogAppender)

struct lfcLogAppender { const struct lfcObject _;
};

struct lfcLogAppender_class { const struct lfcObject_class _;
    method_t print;
};

struct lfcLogAppender_methods {
    int (*print)(lfcLogAppender_t *self, lfcLog_t *log);

    // super
    const lfcObject_methods_t *base;
};

int lfcLogAppender_print(lfcLogAppender_t *self, lfcLog_t *log);

#endif //LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_
