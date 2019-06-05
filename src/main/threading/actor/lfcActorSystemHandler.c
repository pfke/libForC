#include "lfcActorSystemHandler.h"

#include <asm/errno.h>
#include <regex.h>
#include <pthread.h>
#include <stdlib.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActorSystemHandler_t *_lfcActorSystemHandler;
static const lfcActorSystemHandler_class_t *_lfcActorSystemHandler_class;
static lfcActorSystemHandler_methods_t _lfcActorSystemHandler_methods;

static pthread_mutex_t lfcActorSystemHandler_instance_mutex = PTHREAD_MUTEX_INITIALIZER;
static lfcActorSystemHandler_t *lfcActorSystemHandler_instance;


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
lfcActorSystemHandler_t *public_lfcActorSystemHandler_ctor(
    void *_self,
    va_list *app
) {
    lfcActorSystemHandler_t *self = (lfcActorSystemHandler_t *) lfcObject_super_ctor(lfcActorSystemHandler(), _self);

    if (self == 0) {
        return 0;
    }

    self->destroySocketHandler_onExit = false;

    // read args
    self->socketHandler = va_arg(*app, lfcSocketHandler_t *);
    if (self->socketHandler) {
        self->destroySocketHandler_onExit = true;
    }

    self->registeredActorSystems = lfcList_ctor();
    if (!self->registeredActorSystems) { goto err; }

    pthread_mutex_init(&self->lock, NULL);

    return self;

err:
    if (self->registeredActorSystems) { delete(self->registeredActorSystems); }
    free(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcActorSystemHandler_t *public_lfcActorSystemHandler_dtor(
    lfcActorSystemHandler_t *self
) {
    pthread_mutex_lock(&self->lock);

    delete(self->registeredActorSystems);
    if (self->destroySocketHandler_onExit) {
        delete(self->socketHandler);
    }
    self->socketHandler = NULL;

    pthread_mutex_unlock(&self->lock);
    pthread_mutex_destroy(&self->lock);

    return lfcObject_super_dtor(lfcActorSystemHandler(), self);
}

/**
 * Return our socket.
 */
static lfcList_t *public_lfcActorSystemHandler_getRegisteredActorSystems(
    lfcActorSystemHandler_t *self
) {
    return self->registeredActorSystems;
}

/**
 * Return our socket.
 */
static lfcSocketHandler_t *public_lfcActorSystemHandler_getSocketHandler(
    lfcActorSystemHandler_t *self
) {
    // instance bereits angelegt?
    if (!self->socketHandler) {
        // geschützt eine instance anlegen
        if (!pthread_mutex_lock(&self->lock)) {
            if (!self->socketHandler) {
                self->socketHandler = lfcSocketHandler_ctor();
                self->destroySocketHandler_onExit = true;
            }

            pthread_mutex_unlock(&self->lock);
        }
    }

    return self->socketHandler;
}

/**
 */
static int public_lfcActorSystemHandler_registerActorSystem(
    lfcActorSystemHandler_t *self,
    lfcActorSystem_t *actorSystem
) {
    int result;

    if (!actorSystem) { return -EINVAL; }

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = lfcList_add(self->registeredActorSystems, actorSystem);

        pthread_mutex_unlock(&self->lock);
    } else {
        result = -EACCES;
    }

    return result;
}

/**
 */
static int public_lfcActorSystemHandler_unregisterActorSystem(
    lfcActorSystemHandler_t *self,
    lfcActorSystem_t *actorSystem
) {
    int result;

    if (!actorSystem) { return -EINVAL; }

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = lfcList_remove(self->registeredActorSystems, actorSystem);

        pthread_mutex_unlock(&self->lock);
    } else {
        result = -EACCES;
    }

    return result;
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
CLASS_CTOR__START(lfcActorSystemHandler)
        OVERRIDE_METHOD(lfcActorSystemHandler, getRegisteredActorSystems)
        OVERRIDE_METHOD(lfcActorSystemHandler, getSocketHandler)
        OVERRIDE_METHOD(lfcActorSystemHandler, registerActorSystem)
        OVERRIDE_METHOD(lfcActorSystemHandler, unregisterActorSystem)
    CLASS_CTOR__INIT_SUPER(lfcActorSystemHandler, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActorSystemHandler_class_t *lfcActorSystemHandler_class() {
    return _lfcActorSystemHandler_class
        ? _lfcActorSystemHandler_class
        : (_lfcActorSystemHandler_class = (lfcActorSystemHandler_class_t *) new(
            lfcObject_class(), "lfcActorSystemHandler_class", lfcObject_class(), sizeof(lfcActorSystemHandler_class_t),

            lfcObject_ctor, "", impl_lfcActorSystemHandler_class_ctor,

            (void *) 0)
        );
}

const lfcActorSystemHandler_t *lfcActorSystemHandler() {
    return _lfcActorSystemHandler
        ? _lfcActorSystemHandler
        : (_lfcActorSystemHandler = (lfcActorSystemHandler_t *) new(
            lfcActorSystemHandler_class(),
            "lfcActorSystemHandler", lfcObject(), sizeof(lfcActorSystemHandler_t),

            lfcObject_ctor, "ctor", public_lfcActorSystemHandler_ctor,
            lfcObject_dtor, "dtor", public_lfcActorSystemHandler_dtor,

            lfcActorSystemHandler_getRegisteredActorSystems, "getRegisteredActorSystems", public_lfcActorSystemHandler_getRegisteredActorSystems,
            lfcActorSystemHandler_getSocketHandler, "getSocketHandler", public_lfcActorSystemHandler_getSocketHandler,

            lfcActorSystemHandler_registerActorSystem, "registerActorSystem", public_lfcActorSystemHandler_registerActorSystem,
            lfcActorSystemHandler_unregisterActorSystem, "unregisterActorSystem", public_lfcActorSystemHandler_unregisterActorSystem,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActorSystemHandler);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_IMPL_ACCESSOR(lfcActorSystemHandler, getRegisteredActorSystems, lfcList_t *)
lfcOOP_IMPL_ACCESSOR(lfcActorSystemHandler, getSocketHandler, lfcSocketHandler_t *)
lfcOOP_IMPL_ACCESSOR(lfcActorSystemHandler, registerActorSystem, int, lfcActorSystem_t *)
lfcOOP_IMPL_ACCESSOR(lfcActorSystemHandler, unregisterActorSystem, int, lfcActorSystem_t *)

/**
 * Erzeugt eine lfcActorSystemHandler Instanz.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_ctor() {
    return (lfcActorSystemHandler_t *)new(lfcActorSystemHandler(), NULL);
}

/**
 * Erzeugt eine lfcActorSystemHandler Instanz.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_ctor_wSocketHandler(
    const lfcSocketHandler_t *socketHandler
) {
    return (lfcActorSystemHandler_t *)new(lfcActorSystemHandler(), socketHandler);
}

/**
 * Erzeugt eine lfcActorSystemHandler Instanz.
 */
lfcActorSystemHandler_t *lfcActorSystemHandler_singleton () {
    // instance bereits angelegt?
    if (!lfcActorSystemHandler_instance) {
        // geschützt eine instance anlegen
        if (!pthread_mutex_lock(&lfcActorSystemHandler_instance_mutex)) {
            if (!lfcActorSystemHandler_instance) {
                lfcActorSystemHandler_instance = lfcActorSystemHandler_ctor();
            }

            pthread_mutex_unlock(&lfcActorSystemHandler_instance_mutex);
        }
    }

    return lfcActorSystemHandler_instance;
}

/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
