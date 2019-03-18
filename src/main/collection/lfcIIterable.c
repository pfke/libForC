#include "lfcIIterable.h"
#include "../core/lfcIface.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static lfcIIterable_methods_t _lfcIIterable_methods;

IFACE_FORWARD__RET__w_0Param(lfcIIterable, count, size_t)
IFACE_FORWARD__RET__w_1Param(lfcIIterable, exists, int, const void *)
IFACE_FORWARD__RET__w_2Param(lfcIIterable, filter, void *, lfcIIterable_freeOldCollection_e, lambda_fn__retInt__param1_voidPtr)
IFACE_FORWARD__RET__w_2Param(lfcIIterable, filterNot, void *, lfcIIterable_freeOldCollection_e, lambda_fn__retInt__param1_voidPtr)
IFACE_FORWARD__RET__w_1Param(lfcIIterable, find, void *, lambda_fn__retInt__param1_voidPtr)
IFACE_FORWARD__RET__w_1Param(lfcIIterable, foreach, void, lambda_fn__noRet__param1_voidPtr)
IFACE_FORWARD__RET__w_0Param(lfcIIterable, isEmpty, int)
IFACE_FORWARD__RET__w_0Param(lfcIIterable, head, void *)
IFACE_FORWARD__RET__w_0Param(lfcIIterable, last, void *)
IFACE_FORWARD__RET__w_2Param(lfcIIterable, map, void *, lfcIIterable_freeOldCollection_e, lambda_fn__retVoidPtr__param1_voidPtr)
IFACE_FORWARD__RET__w_1Param(lfcIIterable, next, void *, const void *)
IFACE_FORWARD__RET__w_0Param(lfcIIterable, nonEmpty, int)

IFACE_INIT__START(lfcIIterable)
        OVERRIDE_METHOD(lfcIIterable, count)
        OVERRIDE_METHOD(lfcIIterable, exists)
        OVERRIDE_METHOD(lfcIIterable, filter)
        OVERRIDE_METHOD(lfcIIterable, filterNot)
        OVERRIDE_METHOD(lfcIIterable, find)
        OVERRIDE_METHOD(lfcIIterable, foreach)
        OVERRIDE_METHOD(lfcIIterable, isEmpty)
        OVERRIDE_METHOD(lfcIIterable, head)
        OVERRIDE_METHOD(lfcIIterable, last)
        OVERRIDE_METHOD(lfcIIterable, map)
        OVERRIDE_METHOD(lfcIIterable, next)
        OVERRIDE_METHOD(lfcIIterable, nonEmpty)
IFACE_INIT__END()

IFACE_MAKE_METHODS_FUNC(lfcIIterable)
