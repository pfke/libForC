#ifndef LIBFORC_IPC_MQUEUERECEIVER_H
    #define LIBFORC_IPC_MQUEUERECEIVER_H
    #ifdef __cplusplus
    extern "C" {
    #endif

    #include <mqueue.h>
    #include "core/lfcObject.h"


    /*--------------------------------------------------------------------------------------*\
    \*--------------------------------------------------------------------------------------*/
    DEFINE_CLASS(lfcMQueueReceiver);


    /*--------------------------------------------------------------------------------------*\
    \*--------------------------------------------------------------------------------------*/
    struct lfcMQueueReceiver { const struct lfcObject _;
        const char *queue_name;
        int open_flags;
        mode_t open_mode;

        mqd_t our_mqueue;
        struct mq_attr our_mqueue_attr;

        char *buffer;
    };

    struct lfcMQueueReceiver_class { const struct lfcObject_class _;
        method_t receive;
    };

    struct lfcMQueueReceiver_methods {
        const char *(*receive)(lfcMQueueReceiver_t *self, size_t *msg_len, unsigned int *msg_prio);

        // super
        const lfcObject_methods_t *base;
    };

    /**
     * Erzeugt eine lfcMQueueReceiver Instanz.
     */
    lfcMQueueReceiver_t *lfcMQueueReceiver_ctor(
        const char *queue_name,
        int open_flags,
        mode_t open_mode,
        struct mq_attr *open_attr_ptr
    );

    /**
     * Erzeugt eine lfcMQueueReceiver Instanz.
     */
    lfcMQueueReceiver_t *lfcMQueueReceiver_ctorSimple(
        const char *queue_name
    );

    /**
     * API von posix mqueue. (please see man mq_overview)
     */
    const char *lfcMQueueReceiver_receive(
        lfcMQueueReceiver_t *self,
        size_t *msg_len,
        unsigned int *msg_prio
    );


    /******************************************************************************************/
    /* STATIC METHODS                                                                         */
    /******************************************************************************************/


    #ifdef __cplusplus
    }
    #endif
#endif /*--- #ifndef LIBFORC_IPC_MQUEUERECEIVER_H ---*/
