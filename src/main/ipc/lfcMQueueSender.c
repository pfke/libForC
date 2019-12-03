#include "lfcMQueueSender.h"
#include <errno.h>
#include <fcntl.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcMQueueSender_t *_lfcMQueueSender;
static const lfcMQueueSender_class_t *_lfcMQueueSender_class;
static lfcMQueueSender_methods_t _lfcMQueueSender_methods;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static ssize_t public_lfcMQueueSender_send (lfcMQueueSender_t *self, char *msg_ptr, size_t msg_len, unsigned int msg_prio);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
lfcMQueueSender_t *public_lfcMQueueSender_ctor(
    void *_self,
    va_list *app
) {
    lfcMQueueSender_t *self = (lfcMQueueSender_t *) lfcObject_super_ctor(lfcMQueueSender(), _self);
    struct mq_attr *open_attr_ptr;

    if (self == 0) {
        return 0;
    }

    // read args
    self->queue_name = va_arg(*app, const char *);
#ifdef O_CLOEXEC
    self->open_flags = va_arg(*app, int) | O_WRONLY | O_CREAT | O_CLOEXEC;
#else // #ifdef O_CLOEXEC
    self->open_flags = va_arg(*app, int) | O_WRONLY | O_CREAT;
#endif // #else // #ifdef O_CLOEXEC
    self->open_mode = va_arg(*app, int);
    open_attr_ptr = va_arg(*app, struct mq_attr *);
    if (open_attr_ptr) {
        self->our_mqueue_attr = *open_attr_ptr;
    }

    // attach to queue
    errno = 0;
    self->our_mqueue = mq_open(self->queue_name, self->open_flags, self->open_mode, self->our_mqueue_attr);
    if (self->our_mqueue == -1) {
        goto err_open_mqueue;
    }

    // get attrs
    {
        struct mq_attr read_attr;

        memset(&read_attr, 0, sizeof(read_attr));
        errno = 0;
        if (mq_getattr(self->our_mqueue, &read_attr) != 0) {
            goto err_getattr_mqueue;
        }
        memcpy(&self->our_mqueue_attr, &read_attr, sizeof(read_attr));
    }

    return self;

err_getattr_mqueue:
    mq_close(self->our_mqueue);
err_open_mqueue:
    delete(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcMQueueSender_t *public_lfcMQueueSender_dtor(
    lfcMQueueSender_t *self
) {
    mq_close(self->our_mqueue);

    return self;
}

/**
 * API von posix mqueue. (please see man mq_overview)
 */
static ssize_t public_lfcMQueueSender_send(
    lfcMQueueSender_t *self,
    char *msg_ptr,
    size_t msg_len,
    unsigned int msg_prio
) {
    return mq_send(self->our_mqueue, msg_ptr, msg_len, msg_prio);
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
CLASS_CTOR__START(lfcMQueueSender)
        OVERRIDE_METHOD(lfcMQueueSender, send)
    CLASS_CTOR__INIT_SUPER(lfcMQueueSender, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcMQueueSender_class_t *lfcMQueueSender_class() {
    return _lfcMQueueSender_class
           ? _lfcMQueueSender_class
           : (_lfcMQueueSender_class = (lfcMQueueSender_class_t *) new(
            lfcObject_class(), "lfcMQueueSender_class", lfcObject_class(), sizeof(lfcMQueueSender_class_t),

            lfcObject_ctor, "", impl_lfcMQueueSender_class_ctor,

            (void *) 0)
           );
}

const lfcMQueueSender_t *lfcMQueueSender() {
    return _lfcMQueueSender
           ? _lfcMQueueSender
           : (_lfcMQueueSender = (lfcMQueueSender_t *) new(
            lfcMQueueSender_class(),
            "lfcMQueueSender", lfcObject(), sizeof(lfcMQueueSender_t),

            lfcObject_ctor, "ctor", public_lfcMQueueSender_ctor,
            lfcObject_dtor, "dtor", public_lfcMQueueSender_dtor,

            lfcMQueueSender_send, "send", public_lfcMQueueSender_send,

            (void *) 0
        )
           );
}

CLASS_MAKE_METHODS_FUNC(lfcMQueueSender);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_accessor(lfcMQueueSender, send, ssize_t, char *, size_t, unsigned int);


/**
 * Erzeugt eine lfcMQueueSender Instanz.
 */
lfcMQueueSender_t *lfcMQueueSender_ctor(
    const char *queue_name,
    int open_flags,
    int open_mode,
    struct mq_attr *open_attr_ptr
) {
    return (lfcMQueueSender_t *)new(lfcMQueueSender(), queue_name, open_flags, open_mode, open_attr_ptr);
}

/**
 * Erzeugt eine lfcMQueueSender Instanz.
 */
lfcMQueueSender_t *lfcMQueueSender_ctorSimple(
    const char *queue_name
) {
    return lfcMQueueSender_ctor(queue_name, 0, 0644, NULL);
}

/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
