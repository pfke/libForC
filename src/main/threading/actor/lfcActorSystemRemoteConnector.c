#include "lfcActorSystemRemoteConnector.h"

#include <asm/errno.h>
#include <regex.h>
#include <stdlib.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActorSystemRemoteConnector_t *_lfcActorSystemRemoteConnector;
static const lfcActorSystemRemoteConnector_class_t *_lfcActorSystemRemoteConnector_class;
static lfcActorSystemRemoteConnector_methods_t _lfcActorSystemRemoteConnector_methods;


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
lfcActorSystemRemoteConnector_t *public_lfcActorSystemRemoteConnector_ctor(
    void *_self,
    va_list *app
) {
    lfcActorSystemRemoteConnector_t *self = (lfcActorSystemRemoteConnector_t *) lfcObject_super_ctor(lfcActorSystemRemoteConnector(), _self);

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
static lfcActorSystemRemoteConnector_t *public_lfcActorSystemRemoteConnector_dtor(
    lfcActorSystemRemoteConnector_t *self
) {
    delete(self->socket);

    return self;
}

/**
 * Return our socket.
 */
static const lfcSocket_t *public_lfcActorSystemRemoteConnector_getSocket(
    lfcActorSystemRemoteConnector_t *self
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
CLASS_CTOR__START(lfcActorSystemRemoteConnector)
        OVERRIDE_METHOD(lfcActorSystemRemoteConnector, getSocket)
    CLASS_CTOR__INIT_SUPER(lfcActorSystemRemoteConnector, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActorSystemRemoteConnector_class_t *lfcActorSystemRemoteConnector_class() {
    return _lfcActorSystemRemoteConnector_class
        ? _lfcActorSystemRemoteConnector_class
        : (_lfcActorSystemRemoteConnector_class = (lfcActorSystemRemoteConnector_class_t *) new(
            lfcObject_class(), "lfcActorSystemRemoteConnector_class", lfcObject_class(), sizeof(lfcActorSystemRemoteConnector_class_t),

            lfcObject_ctor, "", impl_lfcActorSystemRemoteConnector_class_ctor,

            (void *) 0)
        );
}

const lfcActorSystemRemoteConnector_t *lfcActorSystemRemoteConnector() {
    return _lfcActorSystemRemoteConnector
        ? _lfcActorSystemRemoteConnector
        : (_lfcActorSystemRemoteConnector = (lfcActorSystemRemoteConnector_t *) new(
            lfcActorSystemRemoteConnector_class(),
            "lfcActorSystemRemoteConnector", lfcObject(), sizeof(lfcActorSystemRemoteConnector_t),

            lfcObject_ctor, "ctor", public_lfcActorSystemRemoteConnector_ctor,
            lfcObject_dtor, "dtor", public_lfcActorSystemRemoteConnector_dtor,

            lfcActorSystemRemoteConnector_getSocket, "getSocket", public_lfcActorSystemRemoteConnector_getSocket,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActorSystemRemoteConnector);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_accessor(lfcActorSystemRemoteConnector, getSocket, const lfcSocket_t *)

/**
 * Erzeugt eine lfcActorSystemRemoteConnector Instanz.
 */
lfcActorSystemRemoteConnector_t *lfcActorSystemRemoteConnector_ctor(
    lfcSocket_t *socket
) {
    return (lfcActorSystemRemoteConnector_t *)new(lfcActorSystemRemoteConnector(), socket);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
