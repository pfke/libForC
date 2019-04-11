#include "lfcActorMessage.h"


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActorMessage_t *_lfcActorMessage;
static const lfcActorMessage_class_t *_lfcActorMessage_class;
static lfcActorMessage_methods_t _lfcActorMessage_methods;


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
lfcActorMessage_t *public_lfcActorMessage_ctor(
    void *_self,
    va_list *app
) {
    lfcActorMessage_t *self = (lfcActorMessage_t *) lfcObject_super_ctor(lfcActorMessage(), _self);

    if (self == 0) {
        return 0;
    }

    // read args

    return self;
}

/**
 * Destructor-Funktion.
 */
static lfcActorMessage_t *public_lfcActorMessage_dtor(
    lfcActorMessage_t *self
) {
    return self;
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
CLASS_CTOR__START(lfcActorMessage)
    CLASS_CTOR__INIT_SUPER(lfcActorMessage, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActorMessage_class_t *lfcActorMessage_class() {
    return _lfcActorMessage_class
        ? _lfcActorMessage_class
        : (_lfcActorMessage_class = (lfcActorMessage_class_t *) new(
            lfcObject_class(), "lfcActorMessage_class", lfcObject_class(), sizeof(lfcActorMessage_class_t),

            lfcObject_ctor, "", impl_lfcActorMessage_class_ctor,

            (void *) 0)
        );
}

const lfcActorMessage_t *lfcActorMessage() {
    return _lfcActorMessage
        ? _lfcActorMessage
        : (_lfcActorMessage = (lfcActorMessage_t *) new(
            lfcActorMessage_class(),
            "lfcActorMessage", lfcObject(), sizeof(lfcActorMessage_t),

            lfcObject_ctor, "ctor", public_lfcActorMessage_ctor,
            lfcObject_dtor, "dtor", public_lfcActorMessage_dtor,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActorMessage);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Erzeugt eine lfcActorMessage Instanz.
 */
lfcActorMessage_t *lfcActorMessage_ctor(
) {
    return (lfcActorMessage_t *)new(lfcActorMessage());
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
