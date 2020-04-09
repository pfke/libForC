#include <stdlib.h>
#include <asm/errno.h>
#include "lfcActor.h"
#include "../../threading/lfcThreadMisc.h"


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActor_t *_lfcActor;
static const lfcActor_class_t *_lfcActor_class;
static lfcActor_methods_t _lfcActor_methods;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/

static int private_lfcActor_triggerReceiveFn(lfcActor_t *self);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

/**
 *
 */
static void private_lfcActor_callReceiveFn(
    void *msg
) {
    if (!msg) { return; }

    lfcActorMessage_t *actorMessage = asInstanceOf(lfcActorMessage(), msg);
    if (!actorMessage) { return; }

    lfcActor_t *self = asInstanceOf(lfcActor(), actorMessage->recipient);
    if (!self) { return; }

    self->receive_fn(self, actorMessage);

    delete(msg);

    set_int32_wLocking(&self->receive_fn_is_running, &self->receive_fn_is_running__mutex, LFC_ACTOR_RECEIVE_IS_NOT_RUNNING);
    private_lfcActor_triggerReceiveFn(self);
}

/**
 * This method checks, if a message is in the queue and can be processed.
 */
static int private_lfcActor_triggerReceiveFn(
    lfcActor_t *self
) {
    lfcActorMessage_t *msg;

    pthread_mutex_lock(&self->trigger_receive__mutex);
    {
        if (set_int32_wLocking(&self->receive_fn_is_running, &self->receive_fn_is_running__mutex, LFC_ACTOR_RECEIVE_IS_RUNNING)) {
            // receive-fn is already running
            pthread_mutex_unlock(&self->trigger_receive__mutex);
            return 0;
        }

next_msg:
        // get new message in queue
        msg = lfcQueue_dequeue(self->mailbox);
        if (!msg) {
            set_int32_wLocking(&self->receive_fn_is_running, &self->receive_fn_is_running__mutex, LFC_ACTOR_RECEIVE_IS_NOT_RUNNING);
            pthread_mutex_unlock(&self->trigger_receive__mutex);

            return 1;
        }

        // same system?
        if(!lfcActorSystem_equals_byActorRef(self->actorSystem, msg->recipient)) {
            // drop msg, should be filtered on an earlier state already
            delete(msg);

            goto next_msg;
        }

        pthread_mutex_unlock(&self->trigger_receive__mutex);
    }

    lfcThreadPool_addWorker(
        lfcActorSystem_getThreadPool(self->actorSystem),
        private_lfcActor_callReceiveFn,
        msg,
        0
    );

    return 0;
}


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
    self->actorSystem = va_arg(*app, lfcActorSystem_t *);
    self->receive_fn = va_arg(*app, receive_fn_cb);

    if (!self->receive_fn) { goto err; }
    if (!name) { goto err; }
    if (strstr(name, " ")) { goto err; }
    self->name = strdup(name);
    if (!self->name) { goto err; }

    self->receive_fn_is_running = LFC_ACTOR_RECEIVE_IS_NOT_RUNNING;
    pthread_mutex_init(&self->receive_fn_is_running__mutex, NULL);

    self->mailbox = lfcQueue_ctorWithSize(5, 1);
    if (!self->mailbox) { goto err; }

    return self;
err:
    if (self->name) { free(self->name); }
    if (self->mailbox) { delete(self->mailbox); }
    free(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcActor_t *public_lfcActor_dtor(
    lfcActor_t *self
) {
    if (self->name) { free(self->name); }
    if (self->mailbox) { delete(self->mailbox); }

    pthread_mutex_destroy(&self->receive_fn_is_running__mutex);

    return self;
}

/**
 * Add a message to the actor mailbox.
 * This is an internal method and should only be used by lfcActor*-classes.
 *
 * This function first enqueues the message and then check, if there is currently
 * a message processed by the actor. If not, the receive func will be called.
 */
static int public_lfcActor_addMessageToMailbox(
    lfcActor_t *self,
    lfcActorMessage_t *msg
) {
    if (!msg) { return -EINVAL; }

    lfcQueue_enqueue(self->mailbox, msg);

    return private_lfcActor_triggerReceiveFn(self);
}

/**
 * Returns the actor system
 */
static lfcActorSystem_t *public_lfcActor_getActorSystem(
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

/**
 * Returns the actor ref.
 */
static lfcActorRef_t *public_lfcActor_getRef(
    lfcActor_t *self
) {
    return asInstanceOf(lfcActorRef(), self);
}

/**
 * Send a message to an actor.
 */
static int impl_lfcActor__ActorRef__tell(
    lfcActorRef_t *_self,
    const lfcActorRef_t *recipent,
    const char *msg,
    size_t msg_len
) {
    lfcActor_t *self = asInstanceOf(lfcActor(), _self);
    if (!self) return -EINVAL;
    if (!recipent) return -EINVAL;
    if (!msg) return -EINVAL;
    if (!msg_len) return -EINVAL;

    return lfcActorSystem_tell(self->actorSystem, _self, recipent, msg, msg_len);
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
        OVERRIDE_METHOD(lfcActor, addMessageToMailbox)
        OVERRIDE_METHOD(lfcActor, getActorSystem)
        OVERRIDE_METHOD(lfcActor, getName)
        OVERRIDE_METHOD(lfcActor, getReceiveFn)
        OVERRIDE_METHOD(lfcActor, getRef)
    CLASS_CTOR__INIT_SUPER(lfcActor, lfcActorRef)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActor_class_t *lfcActor_class() {
    return _lfcActor_class
        ? _lfcActor_class
        : (_lfcActor_class = (lfcActor_class_t *) new(
            lfcActorRef_class(), "lfcActor_class", lfcActorRef_class(), sizeof(lfcActor_class_t),

            lfcObject_ctor, "", impl_lfcActor_class_ctor,

            (void *) 0)
        );
}

const lfcActor_t *lfcActor() {
    return _lfcActor
        ? _lfcActor
        : (_lfcActor = (lfcActor_t *) new(
            lfcActor_class(),
            "lfcActor", lfcActorRef(), sizeof(lfcActor_t),

            lfcObject_ctor, "ctor", public_lfcActor_ctor,
            lfcObject_dtor, "dtor", public_lfcActor_dtor,

            lfcActor_addMessageToMailbox, "addMessageToMailbox", public_lfcActor_addMessageToMailbox,
            lfcActor_getActorSystem, "getActorSystem", public_lfcActor_getActorSystem,
            lfcActor_getName, "getName", public_lfcActor_getName,
            lfcActor_getReceiveFn, "getReceiveFn", public_lfcActor_getReceiveFn,
            lfcActor_getRef, "getRef", public_lfcActor_getRef,

            lfcActorRef_tell, "tell", impl_lfcActor__ActorRef__tell,

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
    lfcActorSystem_t *actorSystem,
    receive_fn_cb receive_fn
) {
    return (lfcActor_t *)new(lfcActor(), name, actorSystem, receive_fn);
}

lfcOOP_accessor(lfcActor, addMessageToMailbox, int, lfcActorMessage_t *)
lfcOOP_accessor(lfcActor, getActorSystem, lfcActorSystem_t *)
lfcOOP_accessor(lfcActor, getName, const char *)
lfcOOP_accessor(lfcActor, getReceiveFn, receive_fn_cb)
lfcOOP_accessor(lfcActor, getRef, lfcActorRef_t *)


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
