#ifndef LIBFORC_NET_SOCKET_H_
#define LIBFORC_NET_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/lfcObject.h"
#include "../collection/lfcHashmap.h"
#include "lfcSocketHandler.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocket);

/**
 * Callback der aufgerufen wird, wenn eine eingehende Verbindung angenommen werden soll
 *
 * @param acceptSocket über den die Verbindung angenommen wird
 * @param newSocket ist die neue Verbindung
 * @param context context-ptr, wichtig für den Aufrufer, hier kann die Instanz gespeichert werden
 */
typedef void (*fn_onAcceptConn_cb)(lfcSocket_t *acceptSocket, lfcSocket_t *newSocket, void *context);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocket)

struct lfcSocket { const struct lfcObject _;
    lfcSocketHandler_t *socketHandler;

    /**
     * Hierin wird gespeichert, wer ein listen-Auftrag gegeben hat, und welcher Callback dann aufgerufen werden soll.
     */
    lfcHashmap_t *listen_requestMap;

    /**
     * Ist unser socket file descriptor.
     */
    int fd;
};

struct lfcSocket_class { const struct lfcObject_class _;
    method_t getFd;
    method_t listen;
    method_t read;
    method_t read_async;
    method_t read_job;
    method_t write;
    method_t write_async;
    method_t write_job;
};

struct lfcSocket_methods {
    int (*getFd)(lfcSocket_t *self);
    int (*listen)(lfcSocket_t *self, void *context, fn_onAcceptConn_cb onAcceptConn_cb);
    ssize_t (*read)(lfcSocket_t *self, char *buf, size_t buf_size, int timeout);
    ssize_t (*read_async)(lfcSocket_t *self, char *buf, size_t buf_size, int timeout, unsigned int repeat, fn_onReadComplete_cb onReadComplete);
    ssize_t (*read_job)(lfcSocket_t *self, lfcSocketJobReader_t *job);
    ssize_t (*write)(lfcSocket_t *self, const char *buf, size_t buf_size, int timeout);

    /**
     * Schreiben von Daten über den Socket - asynchron.
     *
     * @param self ist der Instanz-ptr vom socket client
     * @param buf diese Daten sollen geschrieben werden
     * @param buf_size Anzahl der zu schreibenden Bytes
     * @param timeout -1 .. unendlich warten
     *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
     * @param onWriteComplete Callback, wenn das Schreiben fertig ist
     * @return 0 .. Auftrag konnte eingequeued werden
     *          <0 .. Fehler
     */
    ssize_t (*write_async)(lfcSocket_t *self, const char *buf, size_t buf_size, int timeout, fn_onWriteComplete_cb onWriteComplete);

    /**
     * Schreiben von Daten über den Socket - asynchron.
     *
     * @param self ist der Instanz-ptr vom socket client
     * @param job Schreib-Auftrag
     * @return 0 .. Auftrag konnte eingequeued werden
     *          <0 .. Fehler
     */
    ssize_t (*write_job)(lfcSocket_t *self, lfcSocketJobWriter_t *job);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcSocket Instanz.
 */
lfcSocket_t *lfcSocket_ctor(
    lfcSocketHandler_t *socketHandler,
    int fd
);

/**
 * Return the socket fd
 */
int lfcSocket_getFd(lfcSocket_t *self);

/**
 * Aktiviert ein Listen der Verbindung.
 */
int lfcSocket_listen(lfcSocket_t *self, void *context, fn_onAcceptConn_cb onAcceptConn_cb);

/**
 * Lesen von Daten über den Socket.
 * Diese Funktion blockiert, bis entweder alle Daten (<code>buf_size</code>) gelesen wurde oder ein Fehler auftratt.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @return -1 .. Fehler beim Lesen (errno is set)
 *         >=0 Anzahl der gelesenen Daten
 */
ssize_t lfcSocket_read(lfcSocket_t *self, char *buf, size_t buf_size, int timeout);

/**
 * Lesen von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @param repeat Angabe, wie oft der Lese-Befehl ausgeführt werden soll (0 .. endlos, 1 .. one-shot, >1 .. Anzahl)
 * @param onReadComplete Callback, wenn das Lesen fertig ist
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
ssize_t lfcSocket_read_async(lfcSocket_t *self, char *buf, size_t buf_size, int timeout, unsigned int repeat, fn_onReadComplete_cb onReadComplete);

/**
 * Lesen von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job Lese-Auftrag
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
ssize_t lfcSocket_read_job(lfcSocket_t *self, lfcSocketJobReader_t *job);

/**
 * Schreiben von Daten über den Socket.
 * Diese Funktion blockiert, bis entweder alle Daten geschrieben wurde oder beim Schreiben ein Fehler auftratt.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @return -1 .. Fehler beim Schreiben (errno is set)
 *         >=0 Anzahl der geschriebenen Daten
 */
ssize_t lfcSocket_write(lfcSocket_t *self, const char *buf, size_t buf_size, int timeout);

/**
 * Schreiben von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param buf diese Daten sollen geschrieben werden
 * @param buf_size Anzahl der zu schreibenden Bytes
 * @param timeout -1 .. unendlich warten
 *                  >0 .. Anzahl von Sekunden, die gewartet werden sollen
 * @param onWriteComplete Callback, wenn das Schreiben fertig ist
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
ssize_t lfcSocket_write_async(lfcSocket_t *self, const char *buf, size_t buf_size, int timeout, fn_onWriteComplete_cb onWriteComplete);

/**
 * Schreiben von Daten über den Socket - asynchron.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job Schreib-Auftrag
 * @return 0 .. Auftrag konnte eingequeued werden
 *          <0 .. Fehler
 */
ssize_t lfcSocket_write_job(lfcSocket_t *self, lfcSocketJobWriter_t *job);

lfcSocket_t *lfcSocket_connect_tcpStream(
    lfcSocketHandler_t *socketHandler,
    const char *node, const char *port
);

lfcSocket_t *lfcSocket_listenFor_tcpStream(
    lfcSocketHandler_t *socketHandler,
    const char *node, const char *port,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
);

/**
 * @param socketType SOCK_STREAM || SOCK_SEQPACKET
 */
lfcSocket_t *lfcSocket_listenFor_unixDomain(
    lfcSocketHandler_t *socketHandler,
    const char *node, int socketType,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
);

/**
 */
lfcSocket_t *lfcSocket_listenFor_unixDomain_asStream(
    lfcSocketHandler_t *socketHandler,
    const char *node,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
);

/**
 */
lfcSocket_t *lfcSocket_listenFor_unixDomain_asSeqPacket(
    lfcSocketHandler_t *socketHandler,
    const char *node,
    void *context, fn_onAcceptConn_cb onAcceptConn_cb
);

/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_NET_SOCKET_H_ ---*/
