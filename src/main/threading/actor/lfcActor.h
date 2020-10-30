#ifndef LIBFORC_THREADING_ACTOR_LFCACTOR_H
#define LIBFORC_THREADING_ACTOR_LFCACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/lfcObject.h"
#include "../../collection/lfcQueue.h"
#include "../../threading/lfcThreadPool.h"
#include "lfcActorMessage.h"
#include "lfcActorRef.h"
#include <pthread.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActor);
typedef struct lfcActorRef lfcActorRef_t;
typedef struct lfcActorSystem lfcActorSystem_t ;

bool lfcActorSystem_equals_byActorRef(lfcActorSystem_t *self, const lfcActorRef_t *that);
lfcThreadPool_t *lfcActorSystem_getThreadPool(lfcActorSystem_t *self);
int lfcActorSystem_processMessage(
    lfcActorSystem_t *self
);
int lfcActorSystem_tell(
    lfcActorSystem_t *self,
    const lfcActorRef_t *sender,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
);
int lfcActorSystem_tell_byMsg(lfcActorSystem_t *self, lfcActorMessage_t *msg);
typedef void (*receive_fn_cb)(lfcActor_t *, lfcActorMessage_t *);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
#define LFC_ACTOR_RECEIVE_IS_RUNNING        1
#define LFC_ACTOR_RECEIVE_IS_NOT_RUNNING    0


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActor { const struct lfcActorRef _;
    lfcActorSystem_t *actorSystem;

    lfcQueue_t *mailbox;

    /**
     * These var is 
     *   0 .. when the receive-fn is not running
     *   1 .. when the receive-fn is currently running
     * Access on with a mutex to lock parallel access.
     */
    int32_t receive_fn_is_running;
    pthread_mutex_t receive_fn_is_running__mutex;
    pthread_mutex_t trigger_receive__mutex;

    char *name;
    receive_fn_cb receive_fn;
};

struct lfcActor_class { const struct lfcActorRef_class _;
    method_t addMessageToMailbox;

    method_t getActorSystem;

    method_t getName;
    method_t getReceiveFn;
    method_t getRef;
};

struct lfcActor_methods {
    int (*addMessageToMailbox)(lfcActor_t *self, lfcActorMessage_t *msg);
    lfcActorSystem_t * (*getActorSystem)(lfcActor_t *self);
    const char * (*getName)(lfcActor_t *self);
    receive_fn_cb (*getReceiveFn)(lfcActor_t *self);
    lfcActorRef_t * (*getRef)(lfcActor_t *self);

    // super
    const lfcActorRef_methods_t *base;
};

/**
 * Erzeugt eine lfcActor Instanz.
 * Sollte nicht direkt aufgerufen werden.
 */
lfcActor_t *lfcActor_ctor(
    const char *name,
    lfcActorSystem_t *actorSystem,
    receive_fn_cb receive_fn
);

/**
 * Add a message to the actor mailbox.
 * This is an internal method and should only be used by lfcActor*-classes.
 */
int lfcActor_addMessageToMailbox(lfcActor_t *self, lfcActorMessage_t *msg);

/**
 * Return the Actor's actorsystem.
 */
lfcActorSystem_t * lfcActor_getActorSystem(lfcActor_t *self);

/**
 * Return the Actor's name.
 */
const char * lfcActor_getName(lfcActor_t *self);

/**
 * Return the Actor's receive func.
 */
receive_fn_cb lfcActor_getReceiveFn(lfcActor_t *self);

/**
 * Return the actor reference
 */
lfcActorRef_t * lfcActor_getRef(lfcActor_t *self);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCACTOR_H
