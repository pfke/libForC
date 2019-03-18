#ifndef LIBFORC_NET_SOCKETJOBWRITER_H_
#define LIBFORC_NET_SOCKETJOBWRITER_H_

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "../core/lfcObject.h"
#include "lfcSocketJob.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocketJobWriter)

/**
 * Callback der aufgerufen wird, wenn der Lese-Vorgang beendet wurde.
 *
 * @param err Fehlercode (ist der != 0, wird der Job autom. gelöscht)
 * @param read_len Anzahl der gelesenen Bytes
 */
typedef void (*fn_onWriteComplete_cb)(void *context, void *ident, ssize_t written_len, const char *buf);

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcSocketJobWriter { const struct lfcSocketJob _;
    const char *buf;
    size_t buf_len;
    size_t already_written;
    fn_onWriteComplete_cb onWriteComplete;
};

struct lfcSocketJobWriter_class { const struct lfcSocketJob_class _;
};

struct lfcSocketJobWriter_methods {
    // super
    const lfcSocketJob_methods_t *base;
};

/**
 * Einen neuen Writer erstellen
 *
 * @param fd der zu schreibende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param write_buf zu schreibender Buffer
 * @param len_to_write zu schreibende Länge
 * @param Callback der aufgerufen wird, wenn der Schreib-Zugriff fertig ist
 * @param timeout_in_s nach wieviel s soll das Schreiben abgebrochen werden? (0 == kein timeout)
 * @return die erstellte Instanz
 */
lfcSocketJobWriter_t *lfcSocketJobWriter_ctor (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    const char *write_buf,
    size_t len_to_write,
    fn_onWriteComplete_cb onWriteComplete
);

#endif //LIBFORC_NET_SOCKETJOBWRITER_H_
