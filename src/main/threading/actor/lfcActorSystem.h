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
lfcDEFINE_CLASS(lfcActorSystem, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
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
    lfcActorRef_t *, create, (lfcActorSystem_t *self, const char *name, receive_fn_cb receive_fn),

    /**
     * Get the actorsystem name.
     */
    const char *, getName, (lfcActorSystem_t *self),

    /**
     * Returns true, if the that actorsystem is equal to ours.
     */
    bool, equals, (lfcActorSystem_t *self, lfcActorSystem_t *that),
    /**
     * Returns true, if the that actorsystem is equal to ours.
     * Get system from passed actor.
     */
    bool, equals_byActor, (lfcActorSystem_t *self, lfcActor_t *that),
    /**
     * Returns true, if the that actorsystem is equal to ours
     * Get system from passed actorref.
     */
    bool, equals_byActorRef, (lfcActorSystem_t *self, lfcActorRef_t *that),

    /**
     * Send a message from sender to recipient.
     * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
     */
    int, tell, (lfcActorSystem_t *self, const lfcActorRef_t *sender, const lfcActorRef_t *recipient, const char *msg, size_t msg_len),
    /**
     * Send a message from sender to recipient.
     * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
     */
    int, tell_byMsg, (lfcActorSystem_t *self, lfcActorMessage_t *msg),
    /**
     * Send a message from sender to recipient.
     * The messages will be forwarded to the another actorsystem, if the recipient is none of out actors.
     */
    int, tell_noSender, (lfcActorSystem_t *self, const lfcActorRef_t *recipient, const char *msg, size_t msg_len)
    )

/**
 * Erzeugt eine lfcActorSystem Instanz.
 */
lfcActorSystem_t *lfcActorSystem_ctor(
    const char *name
);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTORSYSTEM_H
