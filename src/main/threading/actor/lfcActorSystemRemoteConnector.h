#ifndef LIBFORC_THREADING_ACTOR_LFCREMOTEACTORSYSTEM_H
#define LIBFORC_THREADING_ACTOR_LFCREMOTEACTORSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "../../collection/lfcList.h"
#include "../../core/lfcObject.h"
#include "../../net/lfcSocket.h"
#include "../../threading/lfcThreadPool.h"
#include "lfcActorMessage.h"
#include "lfcActor.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActorSystemRemoteConnector)

struct lfcActorSystemRemoteConnector { const struct lfcObject _;
    lfcSocket_t *socket;
};

struct lfcActorSystemRemoteConnector_class { const struct lfcObject_class _;
    method_t getSocket;
};

struct lfcActorSystemRemoteConnector_methods {
    const lfcSocket_t * (*getSocket)(lfcActorSystemRemoteConnector_t *self);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcActorSystemRemoteConnector Instanz.
 *
 * @param name unter welchem das Actorsystem erreichnbar ist:
 *             1. Lokal: '^[a-z][a-zA-Z0-9\-_]*$'
 *
 *
 *
 *
 * @return
 */
lfcActorSystemRemoteConnector_t *lfcActorSystemRemoteConnector_ctor(
    lfcSocket_t *socket
);

const lfcSocket_t * lfcActorSystemRemoteConnector_getSocket(lfcActorSystemRemoteConnector_t *self);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCREMOTEACTORSYSTEM_H
