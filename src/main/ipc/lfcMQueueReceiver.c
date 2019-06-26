#include "lfcMQueueReceiver.h"
#include <errno.h>
#include "../core/lfcCore.h"
#include <stdlib.h>


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcMQueueReceiver_t *_lfcMQueueReceiver;
static const lfcMQueueReceiver_class_t *_lfcMQueueReceiver_class;
static lfcMQueueReceiver_methods_t _lfcMQueueReceiver_methods;


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static const char *public_lfcMQueueReceiver_receive (lfcMQueueReceiver_t *self, size_t *msg_len, unsigned int *msg_prio);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
lfcMQueueReceiver_t *public_lfcMQueueReceiver_ctor(
    void *_self,
    va_list *app
) {
    lfcMQueueReceiver_t *self = (lfcMQueueReceiver_t *) lfcObject_super_ctor(lfcMQueueReceiver(), _self);
    struct mq_attr *open_attr_ptr;

    if (self == 0) {
        return 0;
    }

    // read args
    self->queue_name = va_arg(*app, const char *);
#ifdef O_CLOEXEC
    self->open_flags = va_arg(*app, int) | O_RDONLY | O_NONBLOCK | O_CLOEXEC | O_CREAT | O_EXCL;
#else // #ifdef O_CLOEXEC
    self->open_flags = va_arg(*app, int) | O_RDONLY | O_NONBLOCK | O_CREAT | O_EXCL;
#endif // #else // #ifdef O_CLOEXEC
    self->open_mode = va_arg(*app, mode_t);
    open_attr_ptr = va_arg(*app, struct mq_attr *);
    if (open_attr_ptr) {
        self->our_mqueue_attr = *open_attr_ptr;
    }

    // attach to queue
    errno = 0;
    self->our_mqueue = mq_open(self->queue_name, self->open_flags, self->open_mode, (open_attr_ptr) ? &self->our_mqueue_attr : NULL);
    if (self->our_mqueue == -1) {
        fprintf(stderr, "%d@%s: could not open mqueue. (errno=%u|'%s')\n", __LINE__, __func__, errno, strerror(errno));
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

    // create_buffer
    self->buffer = calloc(1, (size_t)lfcMAX(self->our_mqueue_attr.mq_msgsize, 64 * 1024));
    if (!self->buffer) {
        fprintf(stderr, "%d@%s: not enough mem to create rx buffer (needed=%zu)\n", __LINE__, __func__, (size_t)self->our_mqueue_attr.mq_msgsize);
        goto err_no_mem;
    }

    return self;

err_no_mem:
err_getattr_mqueue:
    mq_close(self->our_mqueue);
err_open_mqueue:
    delete(self);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcMQueueReceiver_t *public_lfcMQueueReceiver_dtor(
    lfcMQueueReceiver_t *self
) {
    mq_close(self->our_mqueue);

    return self;
}

/**
 * API von posix mqueue. (please see man mq_overview)
 */
static const char *public_lfcMQueueReceiver_receive (
    lfcMQueueReceiver_t *self,
    size_t *msg_len,
    unsigned int *msg_prio
) {
    ssize_t result;

    result = mq_receive(self->our_mqueue, self->buffer, self->our_mqueue_attr.mq_msgsize, msg_prio);
    if (msg_len) {
        *msg_len = (result == -1) ? 0 : (size_t)result;
    }

    return (result == -1) ? NULL : self->buffer;
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
CLASS_CTOR__START(lfcMQueueReceiver)
        OVERRIDE_METHOD(lfcMQueueReceiver, receive)
    CLASS_CTOR__INIT_SUPER(lfcMQueueReceiver, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcMQueueReceiver_class_t *lfcMQueueReceiver_class() {
    return _lfcMQueueReceiver_class
           ? _lfcMQueueReceiver_class
           : (_lfcMQueueReceiver_class = (lfcMQueueReceiver_class_t *) new(
            lfcObject_class(), "lfcMQueueReceiver_class", lfcObject_class(), sizeof(lfcMQueueReceiver_class_t),

            lfcObject_ctor, "", impl_lfcMQueueReceiver_class_ctor,

            (void *) 0)
           );
}

const lfcMQueueReceiver_t *lfcMQueueReceiver() {
    return _lfcMQueueReceiver
           ? _lfcMQueueReceiver
           : (_lfcMQueueReceiver = (lfcMQueueReceiver_t *) new(
            lfcMQueueReceiver_class(),
            "lfcMQueueReceiver", lfcObject(), sizeof(lfcMQueueReceiver_t),

            lfcObject_ctor, "ctor", public_lfcMQueueReceiver_ctor,
            lfcObject_dtor, "dtor", public_lfcMQueueReceiver_dtor,

            lfcMQueueReceiver_receive, "receive", public_lfcMQueueReceiver_receive,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcMQueueReceiver);


/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Erzeugt eine lfcMQueueReceiver Instanz.
 */
lfcMQueueReceiver_t *lfcMQueueReceiver_ctor(
    const char *queue_name,
    int open_flags,
    mode_t open_mode,
    struct mq_attr *open_attr_ptr
) {
    return (lfcMQueueReceiver_t *)new(lfcMQueueReceiver(), queue_name, open_flags, open_mode, open_attr_ptr);
}

/**
 * Erzeugt eine lfcMQueueReceiver Instanz.
 */
lfcMQueueReceiver_t *lfcMQueueReceiver_ctorSimple(
    const char *queue_name
) {
    return lfcMQueueReceiver_ctor(queue_name, 0, 0644, NULL);
}

lfcOOP_accessor(lfcMQueueReceiver, receive, const char *, size_t *, unsigned int *)


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
