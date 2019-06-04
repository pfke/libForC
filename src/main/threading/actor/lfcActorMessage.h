#ifndef LIBFORC_THREADING_ACTOR_LFCACTORMESSAGE_H
#define LIBFORC_THREADING_ACTOR_LFCACTORMESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <mqueue.h>
#include "core/lfcObject.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcActorMessage);

typedef struct lfcActorRef lfcActorRef_t;


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcActorMessage { const struct lfcObject _;
    const lfcActorRef_t *sender;
    const lfcActorRef_t *recipient;
    char *msg;
    size_t msg_len;
};

struct lfcActorMessage_class { const struct lfcObject_class _;
    method_t hasSender;
    method_t getSender;
    method_t getRecipient;
    method_t getMsg;
    method_t getMsgLen;
};

struct lfcActorMessage_methods {
    bool (*hasSender)(lfcActorMessage_t *self);
    const lfcActorRef_t *(*getSender)(lfcActorMessage_t *self);
    const lfcActorRef_t *(*getRecipient)(lfcActorMessage_t *self);
    const char *(*getMsg)(lfcActorMessage_t *self);
    size_t (*getMsgLen)(lfcActorMessage_t *self);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcActorMessage Instanz.
 */
lfcActorMessage_t *lfcActorMessage_ctor(
    const lfcActorRef_t *sender,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
);
lfcActorMessage_t *lfcActorMessage_ctor_noSender(
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
);

bool lfcActorMessage_hasSender(lfcActorMessage_t *self);

/**
 * Returns the sender.
 * Can be NULL, when sent from a non-actor-context.
 */
const lfcActorRef_t *lfcActorMessage_getSender(lfcActorMessage_t *self);
const lfcActorRef_t *lfcActorMessage_getRecipient(lfcActorMessage_t *self);
const char *lfcActorMessage_getMsg(lfcActorMessage_t *self);
size_t lfcActorMessage_getMsgLen(lfcActorMessage_t *self);


#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_ACTOR_LFCACTORMESSAGE_H
