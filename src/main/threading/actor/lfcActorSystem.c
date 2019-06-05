#include "lfcActorSystem.h"

#include <asm/errno.h>
#include <regex.h>
#include <stdlib.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcActorSystem_t *_lfcActorSystem;
static const lfcActorSystem_class_t *_lfcActorSystem_class;
static lfcActorSystem_methods_t _lfcActorSystem_methods;

typedef struct lfcActorSystem_regex_ {
    regex_t localNamingRegex;
    regex_t remoteTCPNamingRegex;
    regex_t remoteMQNamingRegex;
} lfcActorSystem_static_regex_t;
lfcActorSystem_static_regex_t *lfcActorSystem_static_regex = NULL;

/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static void private_lfcActorSystem_dispatchMsg(void *msg);
static int private_lfcActorSystem_tell(lfcActorSystem_t *self, lfcActorMessage_t *msg);

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

static void private_lfcActorSystem_dispatchMsg(
    void *msg
) {
    if (!msg) { return; }

    lfcActorMessage_t *actorMessage = asInstanceOf(lfcActorMessage(), msg);
    if (!actorMessage) { return; }

    lfcActor_t *actor = asInstanceOf(lfcActor(), actorMessage->recipient);
    if (!actor) { return; }

    actor->receive_fn(actor, actorMessage);

    delete(msg);
}

/**
 * Send a message to an actor.
 */
static int private_lfcActorSystem_tell(
    lfcActorSystem_t *self,
    lfcActorMessage_t *msg
) {
    if (!self) { return -EINVAL; }
    if (!msg) { return -EINVAL; }

    // todo: evtl ein remote -System?

    if (!msg->sender || lfcActorSystem_equals_byActorRef(self, (lfcActorRef_t *)msg->recipient)) {
        lfcThreadPool_addWorker(
            self->threadPool,
            private_lfcActorSystem_dispatchMsg,
            msg,
            0
        );
    } else {
        lfcActorSystem_tell_byMsg(
            lfcActor_getActorSystem(asInstanceOf(lfcActor(), msg->recipient)),
            msg
        );
    }

    return 0;
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
lfcActorSystem_t *public_lfcActorSystem_ctor(
    void *_self,
    va_list *app
) {
    lfcActorSystem_t *self = (lfcActorSystem_t *) lfcObject_super_ctor(lfcActorSystem(), _self);

    if (self == 0) {
        return 0;
    }

    // read args
    self->actorSystemHandler = va_arg(*app, const lfcActorSystemHandler_t *);
    const char *name = va_arg(*app, const char *);

    if (!self->actorSystemHandler) { goto err; }

    if (lfcActorSystem_isLocalActorName(name)) {
        self->name = strdup(name);
    } else if (lfcActorSystem_isRemoteActorName_viaMQ(name)) {
        self->name = strdup(name);
    } else if (lfcActorSystem_isRemoteActorName_viaTCP(name)) {
        self->name = strdup(name);
    } else {
        goto err;
    }

    self->actorList = lfcList_ctorWithSize(10, 5);
    if (!self->actorList) { goto err; }

    self->threadPool = lfcThreadPool_create(10, 100, 0);
    if (!self->threadPool) { goto err; }

    lfcActorSystemHandler_registerActorSystem((lfcActorSystemHandler_t *)self->actorSystemHandler, self);

    return self;

err:
    if (self->name) { free(self->name); }
    if (self->actorList) { delete(self->actorList); }
    if (self->threadPool) { lfcThreadPool_destroy(self->threadPool, 0); }

    free(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcActorSystem_t *public_lfcActorSystem_dtor(
    lfcActorSystem_t *self
) {
    if (self->name) { free(self->name); }
    if (self->actorList) { delete(self->actorList); }
    if (self->threadPool) { lfcThreadPool_destroy(self->threadPool, 0); }

    lfcActorSystemHandler_unregisterActorSystem((lfcActorSystemHandler_t *)self->actorSystemHandler, self);

    return lfcObject_super_dtor(lfcActorSystem(), self);
}

/**
 * create an Actor.
 */
static lfcActorRef_t *public_lfcActorSystem_create(
    lfcActorSystem_t *self,
    const char *name,
    receive_fn_cb receive_fn
) {
    if (!name) { return NULL; }
    if (!receive_fn) { return NULL; }

    lfcActor_t *actor= lfcActor_ctor(name, self, receive_fn);
    lfcList_add(self->actorList, actor);

    return asInstanceOf(lfcActorRef(), actor);
}

/**
 * Return name of this system
 */
static const char *public_lfcActorSystem_getName(
    lfcActorSystem_t *self
) {
    return self->name;
}

static bool public_lfcActorSystem_equals(
    lfcActorSystem_t *self,
    const lfcActorSystem_t *that
) {
    if (!that) { return false; }
    if (!isInstanceOf(lfcActorSystem(), that)) { return false; }

    return self == that;
}

static bool public_lfcActorSystem_equals_byActor(
    lfcActorSystem_t *self,
    lfcActor_t *that
) {
    return public_lfcActorSystem_equals(self, lfcActor_getActorSystem(that));
}

static bool public_lfcActorSystem_equals_byActorRef(
    lfcActorSystem_t *self,
    lfcActorRef_t *that
) {
    return public_lfcActorSystem_equals_byActor(self, asInstanceOf(lfcActor(), that));
}

/**
 * Send a message to an actor.
 */
static int public_lfcActorSystem_tell(
    lfcActorSystem_t *self,
    const lfcActorRef_t *sender,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
) {
    return private_lfcActorSystem_tell(self, lfcActorMessage_ctor(sender, recipient, msg, msg_len));
}

/**
 * Send a message to an actor.
 */
static int public_lfcActorSystem_tell_byMsg(
    lfcActorSystem_t *self,
    lfcActorMessage_t *msg
) {
    return private_lfcActorSystem_tell(self, msg);
}

static int public_lfcActorSystem_tell_noSender(
    lfcActorSystem_t *self,
    const lfcActorRef_t *recipient,
    const char *msg,
    size_t msg_len
) {
    return private_lfcActorSystem_tell(self, lfcActorMessage_ctor_noSender(recipient, msg, msg_len));
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
CLASS_CTOR__START(lfcActorSystem)
        OVERRIDE_METHOD(lfcActorSystem, create)
        OVERRIDE_METHOD(lfcActorSystem, getName)

        OVERRIDE_METHOD(lfcActorSystem, equals)
        OVERRIDE_METHOD(lfcActorSystem, equals_byActor)
        OVERRIDE_METHOD(lfcActorSystem, equals_byActorRef)

        OVERRIDE_METHOD(lfcActorSystem, tell)
        OVERRIDE_METHOD(lfcActorSystem, tell_byMsg)
        OVERRIDE_METHOD(lfcActorSystem, tell_noSender)
    CLASS_CTOR__INIT_SUPER(lfcActorSystem, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcActorSystem_class_t *lfcActorSystem_class() {
    return _lfcActorSystem_class
        ? _lfcActorSystem_class
        : (_lfcActorSystem_class = (lfcActorSystem_class_t *) new(
            lfcObject_class(), "lfcActorSystem_class", lfcObject_class(), sizeof(lfcActorSystem_class_t),

            lfcObject_ctor, "", impl_lfcActorSystem_class_ctor,

            (void *) 0)
        );
}

const lfcActorSystem_t *lfcActorSystem() {
    return _lfcActorSystem
        ? _lfcActorSystem
        : (_lfcActorSystem = (lfcActorSystem_t *) new(
            lfcActorSystem_class(),
            "lfcActorSystem", lfcObject(), sizeof(lfcActorSystem_t),

            lfcObject_ctor, "ctor", public_lfcActorSystem_ctor,
            lfcObject_dtor, "dtor", public_lfcActorSystem_dtor,

            lfcActorSystem_create, "create", public_lfcActorSystem_create,
            lfcActorSystem_getName, "getName", public_lfcActorSystem_getName,

            lfcActorSystem_equals, "equals", public_lfcActorSystem_equals,
            lfcActorSystem_equals_byActor, "equals_byActor", public_lfcActorSystem_equals_byActor,
            lfcActorSystem_equals_byActorRef, "equals_byActorRef", public_lfcActorSystem_equals_byActorRef,

            lfcActorSystem_tell, "tell", public_lfcActorSystem_tell,
            lfcActorSystem_tell_byMsg, "tell_byMsg", public_lfcActorSystem_tell_byMsg,
            lfcActorSystem_tell_noSender, "tell_noSender", public_lfcActorSystem_tell_noSender,

            (void *) 0)
        );
}

CLASS_MAKE_METHODS_FUNC(lfcActorSystem);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_IMPL_ACCESSOR(lfcActorSystem, create, lfcActorRef_t *, const char *, receive_fn_cb)
lfcOOP_IMPL_ACCESSOR(lfcActorSystem, getName, const char *)

lfcOOP_IMPL_ACCESSOR(lfcActorSystem, equals, bool, lfcActorSystem_t *);
lfcOOP_IMPL_ACCESSOR(lfcActorSystem, equals_byActor, bool, lfcActor_t *);
lfcOOP_IMPL_ACCESSOR(lfcActorSystem, equals_byActorRef, bool, lfcActorRef_t *);
lfcOOP_IMPL_ACCESSOR(lfcActorSystem, tell, int, const lfcActorRef_t *, const lfcActorRef_t *, const char *, size_t)
lfcOOP_IMPL_ACCESSOR(lfcActorSystem, tell_byMsg, int, lfcActorMessage_t *)
lfcOOP_IMPL_ACCESSOR(lfcActorSystem, tell_noSender, int, const lfcActorRef_t *, const char *, size_t)

/**
 * Erzeugt eine lfcActorSystem Instanz.
 */
lfcActorSystem_t *lfcActorSystem_ctor(
    const char *name
) {
    return lfcActorSystem_ctor_wActorSystemHandler(lfcActorSystemHandler_singleton(), name);
}

/**
 * Erzeugt eine lfcActorSystem Instanz.
 */
lfcActorSystem_t *lfcActorSystem_ctor_wActorSystemHandler(
    const lfcActorSystemHandler_t *actorSystemHandler,
    const char *name
) {
    return (lfcActorSystem_t *)new(lfcActorSystem(), actorSystemHandler, name);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

static bool static_lfcActorSystem_ensureRegexes() {
    if (!lfcActorSystem_static_regex) {
        lfcActorSystem_static_regex = calloc(1, sizeof(*lfcActorSystem_static_regex));
        if (!lfcActorSystem_static_regex) { return false; }

        if (regcomp(&lfcActorSystem_static_regex->localNamingRegex,     lfcActorSystem_LOCALNAMEREGEX,     REG_EXTENDED)) { goto err; }
        if (regcomp(&lfcActorSystem_static_regex->remoteMQNamingRegex,  lfcActorSystem_REMOTEMQNAMEREGEX,  REG_EXTENDED)) { goto err; }
        if (regcomp(&lfcActorSystem_static_regex->remoteTCPNamingRegex, lfcActorSystem_REMOTETCPNAMEREGEX, REG_EXTENDED)) { goto err; }
    }

    return true;

err:
    if (lfcActorSystem_static_regex) { free(lfcActorSystem_static_regex); }

    return false;
}

/**
 * Returns true, if the passed name matches the local name regex patten.
 */
bool lfcActorSystem_isLocalActorName(
    const char *in
) {
    if (!in) { return false; }
    if (!static_lfcActorSystem_ensureRegexes()) { return false; }

    return !regexec(&lfcActorSystem_static_regex->localNamingRegex, in, 0, NULL, 0);
}

/**
 * Returns true, if the passed name matches the local name regex patten.
 */
bool lfcActorSystem_isRemoteActorName_viaMQ(
    const char *in
) {
    if (!in) { return false; }
    if (!static_lfcActorSystem_ensureRegexes()) { return false; }

    return !regexec(&lfcActorSystem_static_regex->remoteMQNamingRegex, in, 0, NULL, 0);
}

/**
 * Returns true, if the passed name matches the local name regex patten.
 */
bool lfcActorSystem_isRemoteActorName_viaTCP(
    const char *in
) {
    if (!in) { return false; }
    if (!static_lfcActorSystem_ensureRegexes()) { return false; }

    return !regexec(&lfcActorSystem_static_regex->remoteTCPNamingRegex, in, 0, NULL, 0);
}
