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

DEFINE_CLASS(lfcActorSystemHandler)

struct lfcActorSystemHandler { const struct lfcObject _;
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
};

struct lfcActorSystemHandler_class { const struct lfcObject_class _;
    method_t getRegisteredActorSystems;
    method_t getSocketHandler;
    method_t registerActorSystem;
    method_t unregisterActorSystem;
};

struct lfcActorSystemHandler_methods {
    lfcList_t * (*getRegisteredActorSystems)(lfcActorSystemHandler_t *self);
    lfcSocketHandler_t * (*getSocketHandler)(lfcActorSystemHandler_t *self);
    int (*registerActorSystem)(lfcActorSystemHandler_t *self, lfcActorSystem_t *actorSystem);
    int (*unregisterActorSystem)(lfcActorSystemHandler_t *self, lfcActorSystem_t *actorSystem);

    // super
    const lfcObject_methods_t *base;
};


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
 */
lfcList_t * lfcActorSystemHandler_getRegisteredActorSystems(lfcActorSystemHandler_t *self);

/**
 * Jeder AcotrSystemHandler besitzt einen Sockethandler über den die RemoteActorSystems kommunizieren.
 */
lfcSocketHandler_t * lfcActorSystemHandler_getSocketHandler(lfcActorSystemHandler_t *self);

int lfcActorSystemHandler_registerActorSystem(lfcActorSystemHandler_t *self, lfcActorSystem_t *actorSystem);
int lfcActorSystemHandler_unregisterActorSystem(lfcActorSystemHandler_t *self, lfcActorSystem_t *actorSystem);

/**
 * Return singleton instance.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_singleton();

#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCACTORSYSTEMHANDLER_H
