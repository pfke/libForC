#ifndef LIBFORC_IPC_MESSAGE_H
    #define LIBFORC_IPC_MESSAGE_H
    #ifdef __cplusplus
    extern "C" {
    #endif

    #include <mqueue.h>
    #include "core/lfcObject.h"


    /*--------------------------------------------------------------------------------------*\
    \*--------------------------------------------------------------------------------------*/
    DEFINE_CLASS(lfcMessage);


    /*--------------------------------------------------------------------------------------*\
    \*--------------------------------------------------------------------------------------*/
    struct lfcMessage { const struct lfcObject _;
        int msg_type;
        int msg_len;

        char *buffer;

    };

    struct lfcMessage_class { const struct lfcObject_class _;
        method_t encode;
    };

    struct lfcMessage_methods {
        size_t (*encode)(lfcMessage_t *self, char *buf, size_t *max_buf_len);

        // super
        const lfcObject_methods_t *base;
    };

    /**
     * Erzeugt eine lfcMessage Instanz.
     */
    lfcMessage_t *lfcMessage_ctor(
        const char *queue_name,
        int open_flags,
        mode_t open_mode,
        struct mq_attr *open_attr_ptr
    );


    /******************************************************************************************/
    /* STATIC METHODS                                                                         */
    /******************************************************************************************/


    #ifdef __cplusplus
    }
    #endif
#endif /*--- #ifndef LIBFORC_IPC_MESSAGE_H ---*/
