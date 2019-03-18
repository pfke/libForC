#ifndef LIBFORC_NET_SOCKETJOBREADERSTREAM_H_
#define LIBFORC_NET_SOCKETJOBREADERSTREAM_H_

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "../core/lfcObject.h"
#include "lfcSocketJob.h"
#include "lfcSocketJobReader.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocketJobReaderStream)


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcSocketJobReaderStream { const struct lfcSocketJobReader _;
    int free_buf_on_exit;
};

struct lfcSocketJobReaderStream_class { const struct lfcSocketJobReader_class _;
};

struct lfcSocketJobReaderStream_methods {
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
 * @param timeout_in_s nach wieviel s soll das Lesen abgebrochen werden? (0 == kein timeout)
 * @param len_to_read zu lesende Länge
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobReaderStream_t *lfcSocketJobReaderStream_ctor (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    size_t len_to_read,
    fn_onReadComplete_cb onReadComplete
);

/**
 * Einen neuen Reader erstellen, der Lese-Buffer wird von uns alloziiert und dann im Callback zurueckgegeben.
 * (Hinweis: Der Reader löst nur einmal aus)
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param timeout_in_s nach wieviel s soll das Lesen abgebrochen werden? (0 == kein timeout)
 * @param read_buf in diesen Buffer soll gelesen werden
 * @param len_to_read zu lesende Länge
 * @param free_buf_on_exit der Buffer soll bei Beendigung gefreeed werden (1 .. ja, 0 .. nein)
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobReaderStream_t *lfcSocketJobReaderStream_ctor_wBuffer (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    char *read_buf,
    size_t len_to_read,
    int free_buf_on_exit,
    fn_onReadComplete_cb onReadComplete
);


#endif //LIBFORC_NET_SOCKETJOBREADERSTREAM_H_
