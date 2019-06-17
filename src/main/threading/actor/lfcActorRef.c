#include <stdlib.h>
#include <asm/errno.h>
#include "lfcActorRef.h"


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActorRef_t *_lfcActorRef;
static const lfcActorRef_class_t *_lfcActorRef_class;
static lfcActorRef_methods_t _lfcActorRef_methods;


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
lfcActorRef_t *public_lfcActorRef_ctor(
    void *_self,
    va_list *app
) {
    lfcActorRef_t *self = (lfcActorRef_t *) lfcObject_super_ctor(lfcActorRef(), _self);

    if (self == 0) {
        return 0;
    }

    return self;
}

/**
 * Destructor-Funktion.
 */
static lfcActorRef_t *public_lfcActorRef_dtor(
    lfcActorRef_t *self
) {
    return lfcObject_super_dtor(lfcActorRef(), self);
}

/**
 * Send a message to an actor.
 */
static int public_lfcActorRef_tell(
    lfcActorRef_t *self,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
) {
    if (!self) return -EINVAL;
    if (!recipient) return -EINVAL;
    if (!msg) return -EINVAL;
    if (!msg_len) return -EINVAL;

    return -1;
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
CLASS_CTOR__START(lfcActorRef)
        OVERRIDE_METHOD(lfcActorRef, tell)
    CLASS_CTOR__INIT_SUPER(lfcActorRef, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActorRef_class_t *lfcActorRef_class() {
    return _lfcActorRef_class
        ? _lfcActorRef_class
        : (_lfcActorRef_class = (lfcActorRef_class_t *) new(
            lfcObject_class(), "lfcActorRef_class", lfcObject_class(), sizeof(lfcActorRef_class_t),

            lfcObject_ctor, "", impl_lfcActorRef_class_ctor,

            (void *) 0)
        );
}

const lfcActorRef_t *lfcActorRef() {
    return _lfcActorRef
        ? _lfcActorRef
        : (_lfcActorRef = (lfcActorRef_t *) new(
            lfcActorRef_class(),
            "lfcActorRef", lfcObject(), sizeof(lfcActorRef_t),

            lfcObject_ctor, "ctor", public_lfcActorRef_ctor,
            lfcObject_dtor, "dtor", public_lfcActorRef_dtor,

            lfcActorRef_tell, "tell", public_lfcActorRef_tell,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActorRef);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_accessor(lfcActorRef, tell, int, const lfcActorRef_t *, const char *, size_t)


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
