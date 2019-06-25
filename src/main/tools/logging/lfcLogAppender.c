#include "lfcLogAppender.h"
#include "../../core/lfcIface.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static lfcLogAppender_methods_t _lfcLogAppender_methods;

IFACE_FORWARD__RET__w_1Param(lfcLogAppender, put, int, lfcLog_t *)

IFACE_INIT__START(lfcLogAppender)
        OVERRIDE_METHOD(lfcLogAppender, put)
IFACE_INIT__END()

IFACE_MAKE_METHODS_FUNC(lfcLogAppender)
