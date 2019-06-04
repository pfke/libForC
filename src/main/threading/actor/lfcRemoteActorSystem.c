#include "lfcRemoteActorSystem.h"

#include <asm/errno.h>
#include <regex.h>
#include <stdlib.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcRemoteActorSystem_t *_lfcRemoteActorSystem;
static const lfcRemoteActorSystem_class_t *_lfcRemoteActorSystem_class;
static lfcRemoteActorSystem_methods_t _lfcRemoteActorSystem_methods;


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
lfcRemoteActorSystem_t *public_lfcRemoteActorSystem_ctor(
    void *_self,
    va_list *app
) {
    lfcRemoteActorSystem_t *self = (lfcRemoteActorSystem_t *) lfcObject_super_ctor(lfcRemoteActorSystem(), _self);

    if (self == 0) {
        return 0;
    }

    // read args
    self->socket = va_arg(*app, lfcSocket_t *);
    if (!self->socket) { goto err; }

    return self;

err:
    if (self->socket) { delete(self->socket); }
    free(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcRemoteActorSystem_t *public_lfcRemoteActorSystem_dtor(
    lfcRemoteActorSystem_t *self
) {
    delete(self->socket);

    return self;
}

/**
 * Return our socket.
 */
static lfcSocket_t *public_lfcRemoteActorSystem_getSocket(
    lfcRemoteActorSystem_t *self
) {
    return self->socket;
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
CLASS_CTOR__START(lfcRemoteActorSystem)
        OVERRIDE_METHOD(lfcRemoteActorSystem, getSocket)
    CLASS_CTOR__INIT_SUPER(lfcRemoteActorSystem, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcRemoteActorSystem_class_t *lfcRemoteActorSystem_class() {
    return _lfcRemoteActorSystem_class
        ? _lfcRemoteActorSystem_class
        : (_lfcRemoteActorSystem_class = (lfcRemoteActorSystem_class_t *) new(
            lfcObject_class(), "lfcRemoteActorSystem_class", lfcObject_class(), sizeof(lfcRemoteActorSystem_class_t),

            lfcObject_ctor, "", impl_lfcRemoteActorSystem_class_ctor,

            (void *) 0)
        );
}

const lfcRemoteActorSystem_t *lfcRemoteActorSystem() {
    return _lfcRemoteActorSystem
        ? _lfcRemoteActorSystem
        : (_lfcRemoteActorSystem = (lfcRemoteActorSystem_t *) new(
            lfcRemoteActorSystem_class(),
            "lfcRemoteActorSystem", lfcObject(), sizeof(lfcRemoteActorSystem_t),

            lfcObject_ctor, "ctor", public_lfcRemoteActorSystem_ctor,
            lfcObject_dtor, "dtor", public_lfcRemoteActorSystem_dtor,

            lfcRemoteActorSystem_getSocket, "getSocket", public_lfcRemoteActorSystem_getSocket,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcRemoteActorSystem);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_IMPL_ACCESSOR(lfcRemoteActorSystem, getSocket, const lfcSocket_t *)

/**
 * Erzeugt eine lfcRemoteActorSystem Instanz.
 */
lfcRemoteActorSystem_t *lfcRemoteActorSystem_ctor(
    lfcSocket_t *socket
) {
    return (lfcRemoteActorSystem_t *)new(lfcRemoteActorSystem(), socket);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
