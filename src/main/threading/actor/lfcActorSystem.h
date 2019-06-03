#ifndef LIBFORC_THREADING_LFCACTORSYSTEM_H
#define LIBFORC_THREADING_LFCACTORSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include <collection/lfcList.h>
#include "core/lfcObject.h"
#include "threading/lfcThreadPool.h"
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

    method_t equals;
    method_t equals_byActor;
    method_t equals_byActorRef;

    method_t tell;
    method_t tell_byMsg;
    method_t tell_noSender;
};

struct lfcActorSystem_methods {
    lfcActorRef_t *(*create)(lfcActorSystem_t *self, const char *name, receive_fn_cb receive_fn);
    const char *(*getName)(lfcActorSystem_t *self);

    bool (*equals)(lfcActorSystem_t *self, lfcActorSystem_t *that);
    bool (*equals_byActor)(lfcActorSystem_t *self, lfcActor_t *that);
    bool (*equals_byActorRef)(lfcActorSystem_t *self, lfcActorRef_t *that);

    int (*tell)(lfcActorSystem_t *self, const lfcActorRef_t *sender, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);
    int (*tell_byMsg)(lfcActorSystem_t *self, lfcActorMessage_t *msg);
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

bool lfcActorSystem_equals(lfcActorSystem_t *self, lfcActorSystem_t *that);
bool lfcActorSystem_equals_byActor(lfcActorSystem_t *self, lfcActor_t *that);
bool lfcActorSystem_equals_byActorRef(lfcActorSystem_t *self, lfcActorRef_t *that);

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

int lfcActorSystem_tell_byMsg(lfcActorSystem_t *self, lfcActorMessage_t *msg);

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
