#include <stdlib.h>
#include "lfcActorSystem.h"


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActorSystem_t *_lfcActorSystem;
static const lfcActorSystem_class_t *_lfcActorSystem_class;
static lfcActorSystem_methods_t _lfcActorSystem_methods;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
lfcActorSystem_t *public_lfcActorSystem_ctor(
    void *_self,
    va_list *app
) {
    lfcActorSystem_t *self = (lfcActorSystem_t *) lfcObject_super_ctor(lfcActorSystem(), _self);

    if (self == 0) {
        return 0;
    }

    // read args
    const char *name = va_arg(*app, const char *);

    if (!name) { goto err; }
    self->name = strdup(name);

    self->actorList = lfcList_ctorWithSize(10, 5);
    if (!self->actorList) { goto err; }

    self->threadPool = lfcThreadPool_create(10, 100, 0);
    if (!self->threadPool) { goto err; }

    return self;

err:
    if (self->name) { free(self->name); }
    if (self->actorList) { delete(self->actorList); }
    if (self->threadPool) { lfcThreadPool_destroy(self->threadPool, 0); }

    free(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcActorSystem_t *public_lfcActorSystem_dtor(
    lfcActorSystem_t *self
) {
    if (self->name) { free(self->name); }
    if (self->actorList) { delete(self->actorList); }
    if (self->threadPool) { lfcThreadPool_destroy(self->threadPool, 0); }

    return self;
}


/**
 * create an Actor.
 */
static lfcActor_t *public_lfcActorSystem_create(
    lfcActorSystem_t *self,
    const char *name,
    receive_fn_cb receive_fn
) {
    if (!name) { return NULL; }
    if (!receive_fn) { return NULL; }

    lfcActor_t *actor= lfcActor_ctor(name, self, receive_fn);
    lfcList_add(self->actorList, actor);

    return actor;
}

/**
 * Return name of this system
 */
static const char *public_lfcActorSystem_getName(
    lfcActorSystem_t *self
) {
    return self->name;
}

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcActorSystem)
        OVERRIDE_METHOD(lfcActorSystem, create)
        OVERRIDE_METHOD(lfcActorSystem, getName)
    CLASS_CTOR__INIT_SUPER(lfcActorSystem, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActorSystem_class_t *lfcActorSystem_class() {
    return _lfcActorSystem_class
        ? _lfcActorSystem_class
        : (_lfcActorSystem_class = (lfcActorSystem_class_t *) new(
            lfcObject_class(), "lfcActorSystem_class", lfcObject_class(), sizeof(lfcActorSystem_class_t),

            lfcObject_ctor, "", impl_lfcActorSystem_class_ctor,

            (void *) 0)
        );
}

const lfcActorSystem_t *lfcActorSystem() {
    return _lfcActorSystem
        ? _lfcActorSystem
        : (_lfcActorSystem = (lfcActorSystem_t *) new(
            lfcActorSystem_class(),
            "lfcActorSystem", lfcObject(), sizeof(lfcActorSystem_t),

            lfcObject_ctor, "ctor", public_lfcActorSystem_ctor,
            lfcObject_dtor, "dtor", public_lfcActorSystem_dtor,

            lfcActorSystem_create, "create", public_lfcActorSystem_create,
            lfcActorSystem_getName, "getName", public_lfcActorSystem_getName,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActorSystem);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Erzeugt eine lfcActorSystem Instanz.
 */
lfcActorSystem_t *lfcActorSystem_ctor(
    const char *name
) {
    return (lfcActorSystem_t *)new(lfcActorSystem(), name);
}


IMPL_API__wRET__w_2PARAM(lfcActorSystem, create, lfcActor_t *, const char *, receive_fn_cb)
IMPL_API__wRET__w_0PARAM(lfcActorSystem, getName, const char *)


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

