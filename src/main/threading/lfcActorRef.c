#include <stdlib.h>
#include "lfcActor.h"


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActor_t *_lfcActor;
static const lfcActor_class_t *_lfcActor_class;
static lfcActor_methods_t _lfcActor_methods;


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
lfcActor_t *public_lfcActor_ctor(
    void *_self,
    va_list *app
) {
    lfcActor_t *self = (lfcActor_t *) lfcObject_super_ctor(lfcActor(), _self);

    if (self == 0) {
        return 0;
    }

    // read args
    const char *name = va_arg(*app, const char *);
    self->actorSystem = va_arg(*app, const lfcActorSystem_t *);
    self->receive_fn = va_arg(*app, receive_fn_cb);

    if (!self->receive_fn) { return NULL; }
    if (!name) { goto err; }
    self->name = strdup(name);

    return self;
err:
    if (self->name) { free(self->name); }

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcActor_t *public_lfcActor_dtor(
    lfcActor_t *self
) {
    if (self->name) { free(self->name); }

    return self;
}

/**
 * Returns the actor system
 */
static const lfcActorSystem_t *public_lfcActor_getActorSystem(
    lfcActor_t *self
) {
    return self->actorSystem;
}

/**
 * Return name of this actor
 */
static const char *public_lfcActor_getName(
    lfcActor_t *self
) {
    return self->name;
}

/**
 * Returns the actor receive fn.
 */
static receive_fn_cb public_lfcActor_getReceiveFn(
    lfcActor_t *self
) {
    return self->receive_fn;
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
CLASS_CTOR__START(lfcActor)
        OVERRIDE_METHOD(lfcActor, getActorSystem)
        OVERRIDE_METHOD(lfcActor, getName)
        OVERRIDE_METHOD(lfcActor, getReceiveFn)
    CLASS_CTOR__INIT_SUPER(lfcActor, lfcActorRef)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActor_class_t *lfcActor_class() {
    return _lfcActor_class
        ? _lfcActor_class
        : (_lfcActor_class = (lfcActor_class_t *) new(
            lfcObject_class(), "lfcActor_class", lfcObject_class(), sizeof(lfcActor_class_t),

            lfcObject_ctor, "", impl_lfcActor_class_ctor,

            (void *) 0)
        );
}

const lfcActor_t *lfcActor() {
    return _lfcActor
        ? _lfcActor
        : (_lfcActor = (lfcActor_t *) new(
            lfcActor_class(),
            "lfcActor", lfcObject(), sizeof(lfcActor_t),

            lfcObject_ctor, "ctor", public_lfcActor_ctor,
            lfcObject_dtor, "dtor", public_lfcActor_dtor,

            lfcActor_getActorSystem, "getActorSystem", public_lfcActor_getActorSystem,
            lfcActor_getName, "getName", public_lfcActor_getName,
            lfcActor_getReceiveFn, "getReceiveFn", public_lfcActor_getReceiveFn,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActor);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Erzeugt eine lfcActor Instanz.
 */
lfcActor_t *lfcActor_ctor(
    const char *name,
    const lfcActorSystem_t *actorSystem,
    receive_fn_cb receive_fn
) {
    return (lfcActor_t *)new(lfcActor(), name, actorSystem, receive_fn);
}

IMPL_API__wRET__w_0PARAM(lfcActor, getActorSystem, const lfcActorSystem_t *)
IMPL_API__wRET__w_0PARAM(lfcActor, getName, const char *)
IMPL_API__wRET__w_0PARAM(lfcActor, getReceiveFn, receive_fn_cb)


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
