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
#include "lfcActorSystemHandler.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
#define lfcActorSystem_LOCALNAMEREGEX           "^[a-z][a-zA-Z0-9_-]*$"         // '-' MUSS der letzte in der Gruppe sein, weil er ansonsten nicht als Symbol interpretiert wird
#define lfcActorSystem_REMOTEMQNAMEREGEX        "^lfcAA.mq://([a-z][a-zA-Z0-9_-]*)@([a-z][a-zA-Z0-9_:-]*)$"
#define lfcActorSystem_REMOTETCPNAMEREGEX       "^lfcAA.tcp://([a-z][a-zA-Z0-9_-]*)@(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]):([0-9]{1,5})$"

lfcOOP_defineClass(lfcActorSystem, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    const lfcActorSystemHandler_t *actorSystemHandler;
    char *name;
    lfcList_t *actorList;
    lfcThreadPool_t *threadPool;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------

    /**
     * create an Actor.
     */
    lfcActorRef_t *, create, (const char *name, receive_fn_cb receive_fn),

    /**
     * Get the actorsystem name.
     */
    const char *, getName, (),

    /**
     * Returns true, if the that actorsystem is equal to ours.
     */
    bool, equals, (lfcActorSystem_t *that),
    /**
     * Returns true, if the that actorsystem is equal to ours.
     * Get system from passed actor.
     */
    bool, equals_byActor, (lfcActor_t *that),
    /**
     * Returns true, if the that actorsystem is equal to ours
     * Get system from passed actorref.
     */
    bool, equals_byActorRef, (lfcActorRef_t *that),

    /**
     * Send a message from sender to recipient.
     * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
     */
    int, tell, (const lfcActorRef_t *sender, const lfcActorRef_t *recipient, const char *msg, size_t msg_len),
    /**
     * Send a message from sender to recipient.
     * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
     */
    int, tell_byMsg, (lfcActorMessage_t *msg),
    /**
     * Send a message from sender to recipient.
     * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
     */
    int, tell_noSender, (const lfcActorRef_t *recipient, const char *msg, size_t msg_len)
    )

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
