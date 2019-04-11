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
};

struct lfcActorSystem_methods {
    lfcActor_t *(*create)(lfcActorSystem_t *self, const char *name, receive_fn_cb receive_fn);
    const char *(*getName)(lfcActorSystem_t *self);

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
lfcActor_t *lfcActorSystem_create(
    lfcActorSystem_t *self,
    const char *name,
    receive_fn_cb receive_fn
);

const char *lfcActorSystem_getName(lfcActorSystem_t *self);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCACTORSYSTEM_H
