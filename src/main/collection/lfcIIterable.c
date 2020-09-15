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

#define OVERRIDE_METHOD2(PREFIX, NAME) \
      if (selector == (method_fn) PREFIX##_##NAME) {                                                                    \
        if (tag) {                                                                                                      \
          self->NAME.tag = tag;                                                                                         \
          self->NAME.selector = selector;                                                                               \
          _##PREFIX##_methods.NAME = PREFIX##_##NAME;                                                                   \
        }                                                                                                               \
        self->NAME.method = method;                                                                                     \
        continue;                                                                                                       \
      }
IFACE_INIT__START(lfcIIterable)
        OVERRIDE_METHOD2(lfcIIterable, count)
        OVERRIDE_METHOD2(lfcIIterable, exists)
        OVERRIDE_METHOD2(lfcIIterable, filter)
        OVERRIDE_METHOD2(lfcIIterable, filterNot)
        OVERRIDE_METHOD2(lfcIIterable, find)
        OVERRIDE_METHOD2(lfcIIterable, foreach)
        OVERRIDE_METHOD2(lfcIIterable, isEmpty)
        OVERRIDE_METHOD2(lfcIIterable, head)
        OVERRIDE_METHOD2(lfcIIterable, last)
        OVERRIDE_METHOD2(lfcIIterable, map)
        OVERRIDE_METHOD2(lfcIIterable, next)
        OVERRIDE_METHOD2(lfcIIterable, nonEmpty)
IFACE_INIT__END()

IFACE_MAKE_METHODS_FUNC(lfcIIterable)
