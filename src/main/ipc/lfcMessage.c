#include "lfcMessage.h"
#include <errno.h>
#include "../core/lfcCore.h"
#include <stdlib.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcMessage_t *_lfcMessage;
static const lfcMessage_class_t *_lfcMessage_class;
static lfcMessage_methods_t _lfcMessage_methods;


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
lfcMessage_t *public_lfcMessage_ctor(
    void *_self,
    va_list *app __attribute__((unused))
) {
    lfcMessage_t *self = (lfcMessage_t *) lfcObject_super_ctor(lfcMessage(), _self);

    if (self == 0) {
        return 0;
    }

    // read args

    return self;

    delete(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcMessage_t *public_lfcMessage_dtor(
    lfcMessage_t *self
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
CLASS_CTOR__START(lfcMessage)
    CLASS_CTOR__INIT_SUPER(lfcMessage, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcMessage_class_t *lfcMessage_class() {
    return _lfcMessage_class
           ? _lfcMessage_class
           : (_lfcMessage_class = (lfcMessage_class_t *) new(
            lfcObject_class(), "lfcMessage_class", lfcObject_class(), sizeof(lfcMessage_class_t),

            lfcObject_ctor, "", impl_lfcMessage_class_ctor,

            (void *) 0)
           );
}

const lfcMessage_t *lfcMessage() {
    return _lfcMessage
           ? _lfcMessage
           : (_lfcMessage = (lfcMessage_t *) new(
            lfcMessage_class(),
            "lfcMessage", lfcObject(), sizeof(lfcMessage_t),

            lfcObject_ctor, "ctor", public_lfcMessage_ctor,
            lfcObject_dtor, "dtor", public_lfcMessage_dtor,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcMessage);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Erzeugt eine lfcMessage Instanz.
 */
lfcMessage_t *lfcMessage_ctor(
    const char *queue_name,
    int open_flags,
    mode_t open_mode,
    struct mq_attr *open_attr_ptr
) {
    return (lfcMessage_t *)new(lfcMessage(), queue_name, open_flags, open_mode, open_attr_ptr);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
