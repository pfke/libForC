#ifndef LIBFORC_THREADING_LFCACTORSYSTEM_H
#define LIBFORC_THREADING_LFCACTORSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include <collection/lfcList.h>
#include "core/lfcObject.h"
#include "lfcThreadPool.h"
#include "lfcActorMessage.h"
#include "lfcActor.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActorSystem);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActorSystem { const struct lfcObject _;
    char *name;

    lfcList_t *actorList;
    lfcThreadPool_t *threadPool;
};

struct lfcActorSystem_class { const struct lfcObject_class _;
    method_t create;
    method_t getName;

    method_t tell;
    method_t tell_noSender;
};

struct lfcActorSystem_methods {
    lfcActorRef_t *(*create)(lfcActorSystem_t *self, const char *name, receive_fn_cb receive_fn);
    const char *(*getName)(lfcActorSystem_t *self);

    int (*tell)(lfcActorSystem_t *self, const lfcActorRef_t *sender, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);
    int (*tell_noSender)(lfcActorSystem_t *self, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcActorSystem Instanz.
 */
lfcActorSystem_t *lfcActorSystem_ctor(
    const char *name
);

/**
 * create an Actor.
 */
lfcActorRef_t *lfcActorSystem_create(
    lfcActorSystem_t *self,
    const char *name,
    receive_fn_cb receive_fn
);

const char *lfcActorSystem_getName(lfcActorSystem_t *self);

/**
 * Send a message to an actor.
 */
int lfcActorSystem_tell(
    lfcActorSystem_t *self,
    const lfcActorRef_t *sender,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
);

int lfcActorSystem_tell_noSender(
    lfcActorSystem_t *self,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTORSYSTEM_H
