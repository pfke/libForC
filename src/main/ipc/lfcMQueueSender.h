#ifndef LIBFORC_IPC_MQUEUESENDER_H
    #define LIBFORC_IPC_MQUEUESENDER_H
    #ifdef __cplusplus
    extern "C" {
    #endif

    #include "../core/lfcObject.h"
    #include <mqueue.h>


    /*--------------------------------------------------------------------------------------*\
    \*--------------------------------------------------------------------------------------*/
    DEFINE_CLASS(lfcMQueueSender);


    /*--------------------------------------------------------------------------------------*\
    \*--------------------------------------------------------------------------------------*/
    struct lfcMQueueSender { const struct lfcObject _;
        const char *queue_name;
        int open_flags;
        int open_mode;

        mqd_t our_mqueue;
        struct mq_attr our_mqueue_attr;
    };

    struct lfcMQueueSender_class { const struct lfcObject_class _;
        method_t send;
        method_t timedsend;
    };

    struct lfcMQueueSender_methods {
        ssize_t (*send)(lfcMQueueSender_t *self, char *msg_ptr, size_t msg_len, unsigned int msg_prio);
        ssize_t (*timedsend)(lfcMQueueSender_t *self, char *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct timespec *abs_timeout);

        int (*notify)(lfcMQueueSender_t *self, const struct sigevent *sevp);

        // super
        const lfcObject_methods_t *base;
    };

    /**
     * Erzeugt eine lfcMQueueSender Instanz.
     */
    lfcMQueueSender_t *lfcMQueueSender_ctor(
        const char *queue_name,
        int open_flags,
        int open_mode,
        struct mq_attr *open_attr_ptr
    );

    /**
     * Erzeugt eine lfcMQueueSender Instanz.
     */
    lfcMQueueSender_t *lfcMQueueSender_ctorSimple(
        const char *queue_name
    );

    /**
     * API von posix mqueue. (please see man mq_overview)
     */
    ssize_t lfcMQueueSender_send(
        lfcMQueueSender_t *self,
        char *msg_ptr,
        size_t msg_len,
        unsigned int msg_prio
    );

    /**
     * API von posix mqueue. (please see man mq_overview)
     */
    ssize_t lfcMQueueSender_timedsend(
        lfcMQueueSender_t *self,
        char *msg_ptr,
        size_t msg_len,
        unsigned int msg_prio,
        const struct timespec *abs_timeout
    );


    /******************************************************************************************/
    /* STATIC METHODS                                                                         */
    /******************************************************************************************/


    #ifdef __cplusplus
    }
    #endif
#endif /*--- #ifndef LIBFORC_IPC_MQUEUESENDER_H ---*/
