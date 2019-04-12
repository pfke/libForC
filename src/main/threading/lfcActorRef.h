#ifndef LIBFORC_THREADING_LFCACTOR_H
#define LIBFORC_THREADING_LFCACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "core/lfcObject.h"
#include "lfcActorMessage.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActorRef);
DEFINE_CLASS(lfcActor);

typedef struct lfcActorSystem lfcActorSystem_t ;
typedef void (*receive_fn_cb)(lfcActorSystem_t *, lfcActor_t *, lfcActorMessage_t *);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActorRef { const struct lfcObject _;
};

struct lfcActorRef_class { const struct lfcObject_class _;
};

struct lfcActorRef_methods {
    // super
    const lfcObject_methods_t *base;
};


struct lfcActor { const struct lfcActorRef _;
    const lfcActorSystem_t *actorSystem;
    char *name;
    receive_fn_cb receive_fn;
};

struct lfcActor_class { const struct lfcActorRef_class _;
    method_t getActorSystem;
    method_t getName;
    method_t getReceiveFn;
};

struct lfcActor_methods {
    const lfcActorSystem_t *(*getActorSystem)(lfcActor_t *self);
    const char *(*getName)(lfcActor_t *self);
    receive_fn_cb (*getReceiveFn)(lfcActor_t *self);

    // super
    const lfcActorRef_methods_t *base;
};

/**
 * Erzeugt eine lfcActor Instanz.
 */
lfcActor_t *lfcActor_ctor(
    const char *name,
    const lfcActorSystem_t *actorSystem,
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

/**
 * Returns the actor receive fn.
 */
receive_fn_cb lfcActor_getReceiveFn(lfcActor_t *self);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTOR_H
