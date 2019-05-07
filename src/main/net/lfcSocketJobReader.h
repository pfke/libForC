#ifndef LIBFORC_NET_SOCKETJOBREADER_H_
#define LIBFORC_NET_SOCKETJOBREADER_H_

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "../core/lfcObject.h"
#include "lfcSocketJob.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocketJobReader)

/**
 * Callback der aufgerufen wird, wenn der Lese-Vorgang beendet wurde.
 *
 * @param context context-ptr, wichtig für den Aufrufer, hir kann die Instanz gespeichert werden
 * @param ident ist ein Wert, um den Leseauftrag identifizieren zu können
 * @param read_len Anzahl der gelesenen Bytes (bei <0 liegt ein Fehler vor)
 * @param buf ist ein Ptr auf die gelesenen Bytes (wurde der Buffer nicht mitgegeben, ist er vom Aufrufer freizugeben)
 */
typedef void (*fn_onReadComplete_cb)(void *context, void *ident, ssize_t read_len, char *read_buf);

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcSocketJobReader { const struct lfcSocketJob _;
    char *buf;
    size_t buf_len;
    size_t already_read;
    fn_onReadComplete_cb onReadComplete;
    bool free_buf;
};

struct lfcSocketJobReader_class { const struct lfcSocketJob_class _;
};

struct lfcSocketJobReader_methods {
    // super
    const lfcSocketJob_methods_t *base;
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
lfcSocketJobReader_t *lfcSocketJobReader_ctor (
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
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobReader_t *lfcSocketJobReader_ctor_wBuffer (
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    char *read_buf,
    size_t len_to_read,
    fn_onReadComplete_cb onReadComplete
);

/**
 * Einen neuen Reader erstellen, der Lese-Buffer wird von uns alloziiert und dann im Callback zurueckgegeben.
 * Dieser Reader läuft immer und immer wieder und löäscht sich nicht nach einem Empfang.
 *
 * @param fd der zu lesende fd
 * @param context context, mit dem der fertig-callback aufgerufen werden soll
 * @param ident ein vom Aufrufer zu vergebender Identifier
 * @param timeout_in_s nach wieviel s soll das Lesen abgebrochen werden? (0 == kein timeout)
 * @param repeat Angabe, wie oft der Lese-Befehl ausgeführt werden soll (0 .. endlos, 1 .. one-shot, >1 .. Anzahl)
 * @param read_buf in diesen Buffer soll gelesen werden
 * @param len_to_read zu lesende Länge
 * qparam free_buf soll der uebergebene buffer im dtor gefreed werden?
 * @param Callback der aufgerufen wird, wenn der Lese-Zugriff fertig ist
 * @return die erstellte Instanz
 */
lfcSocketJobReader_t *lfcSocketJobReader_ctor_wRepeat(
    int fd,
    void *context,
    void *ident,
    int timeout_in_s,
    unsigned int repeat,
    char *read_buf,
    size_t len_to_read,
    bool free_buf,
    fn_onReadComplete_cb onReadComplete
);


#endif //LIBFORC_NET_SOCKETJOBREADER_H_
