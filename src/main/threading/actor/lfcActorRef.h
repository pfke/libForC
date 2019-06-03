#ifndef LIBFORC_THREADING_LFCACTORRef_H
#define LIBFORC_THREADING_LFCACTORRef_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "core/lfcObject.h"
#include "lfcActorMessage.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActorRef);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActorRef { const struct lfcObject _; };

struct lfcActorRef_class { const struct lfcObject_class _;
    method_t tell;
};

struct lfcActorRef_methods {
    int (*tell)(lfcActorRef_t *self, const lfcActorRef_t *recipent, const char *msg, size_t msg_len);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcActorRef Instanz.
 */
lfcActorRef_t *lfcActorRef_ctor();

/**
 * Send a message to an actor.
 */
int lfcActorRef_tell(
    lfcActorRef_t *self,
    const lfcActorRef_t *recipent,
    const char *msg,
    size_t msg_len
);

#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTORREF_H
