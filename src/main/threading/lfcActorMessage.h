#ifndef LIBFORC_THREADING_LFCACTORMESSAGE_H
#define LIBFORC_THREADING_LFCACTORMESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "core/lfcObject.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActorMessage);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActorMessage { const struct lfcObject _;
};

struct lfcActorMessage_class { const struct lfcObject_class _;
};

struct lfcActorMessage_methods {
    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcActorMessage Instanz.
 */
lfcActorMessage_t *lfcActorMessage_ctor(
);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTORMESSAGE_H
