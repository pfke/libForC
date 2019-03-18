#ifndef LIBFORC_NET_SOCKETJOBACCEPTCONN_H_
#define LIBFORC_NET_SOCKETJOBACCEPTCONN_H_

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "../core/lfcObject.h"
#include "lfcSocketJob.h"
#include "lfcSocketJobReader.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocketJobAcceptConn)


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcSocketJobAcceptConn { const struct lfcSocketJobReader _;
};

struct lfcSocketJobAcceptConn_class { const struct lfcSocketJobReader_class _;
};

struct lfcSocketJobAcceptConn_methods {
    // super
    const lfcSocketJobReader_methods_t *base;
};

/**
 * Einen neuen Reader erstellen.
 * (Hinweis: Der Reader löst nur einmal aus)
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobAcceptConn_t *lfcSocketJobAcceptConn_ctor (
    int fd,
    void *context,
    void *ident,
    fn_onReadComplete_cb onReadComplete
);


#endif //LIBFORC_NET_SOCKETJOBACCEPTCONN_H_
