#ifndef LIBFORC_THREADING_LFCACTOR_H
#define LIBFORC_THREADING_LFCACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "core/lfcObject.h"
#include "lfcActorMessage.h"
#include "lfcActorRef.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActor);
typedef struct lfcActorRef lfcActorRef_t;
typedef struct lfcActorSystem lfcActorSystem_t ;
int lfcActorSystem_tell(
    lfcActorSystem_t *self,
    const lfcActorRef_t *sender,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
);
typedef void (*receive_fn_cb)(lfcActor_t *, lfcActorMessage_t *);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActor { const struct lfcActorRef _;
    lfcActorSystem_t *actorSystem;
    char *name;
    receive_fn_cb receive_fn;
};

struct lfcActor_class { const struct lfcActorRef_class _;
    method_t getActorSystem;
    method_t getName;
    method_t getReceiveFn;
    method_t getRef;
};

struct lfcActor_methods {
    const lfcActorSystem_t *(*getActorSystem)(lfcActor_t *self);
    const char *(*getName)(lfcActor_t *self);
    receive_fn_cb (*getReceiveFn)(lfcActor_t *self);
    lfcActorRef_t *(*getRef)(lfcActor_t *self);

    // super
    const lfcActorRef_methods_t *base;
};

/**
 * Erzeugt eine lfcActor Instanz.
 */
lfcActor_t *lfcActor_ctor(
    const char *name,
    lfcActorSystem_t *actorSystem,
    receive_fn_cb receive_fn
);

/**
 * Returns the actor system
 */
const lfcActorSystem_t *lfcActor_getActorSystem(lfcActor_t *self);

/**
 * Returns the actor name
 */
const char *lfcActor_getName(lfcActor_t *self);

lfcActorRef_t *lfcActor_getRef(lfcActor_t *self);

/**
 * Returns the actor receive fn.
 */
receive_fn_cb lfcActor_getReceiveFn(lfcActor_t *self);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTOR_H
