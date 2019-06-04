#ifndef LIBFORC_THREADING_ACTOR_LFCREMOTEACTORSYSTEM_H
#define LIBFORC_THREADING_ACTOR_LFCREMOTEACTORSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include <collection/lfcList.h>
#include "core/lfcObject.h"
#include "net/lfcSocket.h"
#include "threading/lfcThreadPool.h"
#include "lfcActorMessage.h"
#include "lfcActor.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcDEFINE_CLASS(lfcRemoteActorSystem, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    lfcSocket_t *socket;
    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------

    /**
     */
    const lfcSocket_t *, getSocket, (lfcRemoteActorSystem_t *self)
    )

/**
 * Erzeugt eine lfcRemoteActorSystem Instanz.
 *
 * @param name unter welchem das Actorsystem erreichnbar ist:
 *             1. Lokal: '^[a-z][a-zA-Z0-9\-_]*$'
 *
 *
 *
 *
 * @return
 */
lfcRemoteActorSystem_t *lfcRemoteActorSystem_ctor(
    lfcSocket_t *socket
);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCREMOTEACTORSYSTEM_H
