#include <stdlib.h>
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
    self->sender = va_arg(*app, const lfcActorRef_t *);
    self->recipient = va_arg(*app, const lfcActorRef_t *);
    const char *msg = va_arg(*app, const char *);
    self->msg_len =  va_arg(*app, size_t);

    if (!self->recipient) { goto err; }
    if (!msg) { goto err; }
    self->msg = strdup(msg);
    if (!self->msg) { goto err; }
    if (!self->msg_len) { goto err; }

    return self;

err:
    if (self->msg) { free(self->msg); }
    free(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcActorMessage_t *public_lfcActorMessage_dtor(
    lfcActorMessage_t *self
) {
    if (self->msg) { free(self->msg); }

    return self;
}

static bool public_lfcActorMessage_hasSender(
    lfcActorMessage_t *self
) {
    return self->sender != NULL;
}

static const lfcActorRef_t *public_lfcActorMessage_getSender(
    lfcActorMessage_t *self
) {
    return self->sender;
}

static const lfcActorRef_t *public_lfcActorMessage_getRecipient(
    lfcActorMessage_t *self
) {
    return self->recipient;
}

static const char *public_lfcActorMessage_getMsg(
    lfcActorMessage_t *self
) {
    return self->msg;
}

static size_t public_lfcActorMessage_getMsgLen(
    lfcActorMessage_t *self
) {
    return self->msg_len;
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
        OVERRIDE_METHOD(lfcActorMessage, hasSender)
        OVERRIDE_METHOD(lfcActorMessage, getSender)
        OVERRIDE_METHOD(lfcActorMessage, getRecipient)
        OVERRIDE_METHOD(lfcActorMessage, getMsg)
        OVERRIDE_METHOD(lfcActorMessage, getMsgLen)
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

            lfcActorMessage_hasSender, "hasSender", public_lfcActorMessage_hasSender,
            lfcActorMessage_getSender, "getSender", public_lfcActorMessage_getSender,
            lfcActorMessage_getRecipient, "getRecipient", public_lfcActorMessage_getRecipient,
            lfcActorMessage_getMsg, "getMsg", public_lfcActorMessage_getMsg,
            lfcActorMessage_getMsgLen, "getMsgLen", public_lfcActorMessage_getMsgLen,

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
    const lfcActorRef_t *sender,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
) {
    return (lfcActorMessage_t *)new(lfcActorMessage(), sender, recipient, msg, msg_len);
}

lfcActorMessage_t *lfcActorMessage_ctor_noSender(
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
) {
    return lfcActorMessage_ctor (NULL, recipient, msg, msg_len );
}

lfcOOP_IMPL_ACCESSOR(lfcActorMessage, hasSender, bool)
lfcOOP_IMPL_ACCESSOR(lfcActorMessage, getSender, const lfcActorRef_t *)
lfcOOP_IMPL_ACCESSOR(lfcActorMessage, getRecipient, const lfcActorRef_t *)
lfcOOP_IMPL_ACCESSOR(lfcActorMessage, getMsg, const char *)
lfcOOP_IMPL_ACCESSOR(lfcActorMessage, getMsgLen, size_t)


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
