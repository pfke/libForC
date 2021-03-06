#ifndef LIBFORC_THREADING_ACTOR_LFCACTORSYSTEM_H
#define LIBFORC_THREADING_ACTOR_LFCACTORSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "../../collection/lfcList.h"
#include "../../core/lfcObject.h"
#include "../../threading/lfcThreadPool.h"
#include "lfcActor.h"
#include "lfcActorMessage.h"
#include "lfcActorRef.h"
#include "lfcActorSystemHandler.h"
#include <stdbool.h>
#include <stdlib.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
#define lfcActorSystem_LOCALNAMEREGEX           "^[a-z][a-zA-Z0-9_-]*$"         // '-' MUSS der letzte in der Gruppe sein, weil er ansonsten nicht als Symbol interpretiert wird
#define lfcActorSystem_REMOTEMQNAMEREGEX        "^lfcAA.mq://([a-z][a-zA-Z0-9_-]*)@([a-z][a-zA-Z0-9_:-]*)$"
#define lfcActorSystem_REMOTETCPNAMEREGEX       "^lfcAA.tcp://([a-z][a-zA-Z0-9_-]*)@(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]):([0-9]{1,5})$"

DEFINE_CLASS(lfcActorSystem)

struct lfcActorSystem { const struct lfcObject _;
    const lfcActorSystemHandler_t *actorSystemHandler;
    char *name;
    lfcList_t *actorList;
    lfcThreadPool_t *threadPool;
};

struct lfcActorSystem_class { const struct lfcObject_class _;
    method_t createActor;
    method_t getName;
    method_t getThreadPool;
    method_t equals;
    method_t equals_byActor;
    method_t equals_byActorRef;
    method_t tell;
    method_t tell_byMsg;
    method_t tell_noSender;
};

struct lfcActorSystem_methods {
    lfcActorRef_t *(*createActor)(lfcActorSystem_t *self, const char *name, receive_fn_cb receive_fn);
    const char *(*getName)(lfcActorSystem_t *self);
    lfcThreadPool_t *(*getThreadPool)(lfcActorSystem_t *self);
    bool (*equals)(lfcActorSystem_t *self, const lfcActorSystem_t *that);
    bool (*equals_byActor)(lfcActorSystem_t *self, const lfcActor_t *that);
    bool (*equals_byActorRef)(lfcActorSystem_t *self, const lfcActorRef_t *that);
    int (*tell)(lfcActorSystem_t *self, const lfcActorRef_t *sender, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);
    int (*tell_byMsg)(lfcActorSystem_t *self, lfcActorMessage_t *msg);
    int (*tell_noSender)(lfcActorSystem_t *self, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcActorSystem Instanz.
 *
 * @param name unter welchem das Actorsystem erreichnbar ist:
 *             1. Lokal
 *             2. MessageQueue
 *             3. TCP
 */
lfcActorSystem_t *lfcActorSystem_ctor(
    const char *name
);

/**
 * Erzeugt eine lfcActorSystem Instanz.
 */
lfcActorSystem_t *lfcActorSystem_ctor_wActorSystemHandler(
    const lfcActorSystemHandler_t *actorSystemHandler,
    const char *name
);

/**
 * create an Actor.
 */
lfcActorRef_t *lfcActorSystem_createActor(lfcActorSystem_t *self, const char *name, receive_fn_cb receive_fn);

/**
 * Get the actorsystem name.
 */
const char *lfcActorSystem_getName(lfcActorSystem_t *self);

/**
 * Get the actorsystem name.
 */
lfcThreadPool_t *lfcActorSystem_getThreadPool(lfcActorSystem_t *self);

/**
 * Returns true, if the that actorsystem is equal to ours.
 */
bool lfcActorSystem_equals(lfcActorSystem_t *self, const lfcActorSystem_t *that);
/**
 * Returns true, if the that actorsystem is equal to ours.
 * Get system from passed actor.
 */
bool lfcActorSystem_equals_byActor(lfcActorSystem_t *self, const lfcActor_t *that);
/**
 * Returns true, if the that actorsystem is equal to ours
 * Get system from passed actorref.
 */
bool lfcActorSystem_equals_byActorRef(lfcActorSystem_t *self, const lfcActorRef_t *that);

/**
 * Send a message from sender to recipient.
 * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
 */
int lfcActorSystem_tell(lfcActorSystem_t *self, const lfcActorRef_t *sender, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);
/**
 * Send a message from sender to recipient.
 * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
 */
int lfcActorSystem_tell_byMsg(lfcActorSystem_t *self, lfcActorMessage_t *msg);
/**
 * Send a message from sender to recipient.
 * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
 */
int lfcActorSystem_tell_noSender(lfcActorSystem_t *self, const lfcActorRef_t *recipient, const char *msg, size_t msg_len);

/**
 * Returns true, if the passed name matches the local name regex patten.
 *
 * @param in ist der zu prüfende String: '^[a-z][a-zA-Z0-9\-_]*$'
 * @return
 */
bool lfcActorSystem_isLocalActorName(
    const char *in
);

/**
 * Returns true, if the passed name matches the local name regex patten.
 *
 * @param in ist der zu prüfende String: '^lfcAA.mq://([a-z][a-zA-Z0-9_-]*)@([a-z][a-zA-Z0-9_-:]*)$'
 * @return
 */
bool lfcActorSystem_isRemoteActorName_viaMQ(
    const char *in
);

/**
 * Returns true, if the passed name matches the local name regex patten.
 *
 * @param in ist der zu prüfende String: '^lfcAA.tcp://([a-z][a-zA-Z0-9_-]*)@(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]):([0-9]{1,5})$'
 * @return
 */
bool lfcActorSystem_isRemoteActorName_viaTCP(
    const char *in
);

#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCACTORSYSTEM_H
