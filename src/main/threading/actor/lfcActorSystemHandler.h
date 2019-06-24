#ifndef LIBFORC_THREADING_ACTOR_LFCACTORSYSTEMHANDLER_H
#define LIBFORC_THREADING_ACTOR_LFCACTORSYSTEMHANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include <stdbool.h>
#include "../../collection/lfcList.h"
#include "../../core/lfcObject.h"
#include "../../net/lfcSocketHandler.h"
#include "../../threading/lfcThreadPool.h"
#include "lfcActorMessage.h"
#include "lfcActor.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
typedef struct lfcActorSystem lfcActorSystem_t ;

lfcOOP_defineClass(lfcActorSystemHandler, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    pthread_mutex_t lock;
    lfcList_t *registeredActorSystems;

    /**
     * Sockethandler für die RemoteActorSysteme.
     * Dieser ist lazy, d.h. erst beim Zugriff darauf, wird er angelegt
     *
     * !!! also unbedingt accessor-Methode nutzen !!!
     */
    lfcSocketHandler_t *socketHandler;
    bool destroySocketHandler_onExit;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------

    /**
     */
    lfcList_t *, getRegisteredActorSystems, (),

    /**
     * Jeder AcotrSystemHandler besitzt einen Sockethandler über den die RemoteActorSystems kommunizieren.
     */
    lfcSocketHandler_t *, getSocketHandler, (),

    int, registerActorSystem, (lfcActorSystem_t *actorSystem),
    int, unregisterActorSystem, (lfcActorSystem_t *actorSystem)
    )

/**
 * Erzeugt eine lfcActorSystemHandler Instanz.
 * Es wird bei Bedarf ein SocketHandler erzeugt.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_ctor();

/**
 * Erzeugt eine lfcActorSystemHandler Instanz.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_ctor_wSocketHandler(
    const lfcSocketHandler_t *socketHandler
);

/**
 * Return singleton instance.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_singleton();

#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCACTORSYSTEMHANDLER_H
