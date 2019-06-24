#ifndef LIBFORC_NET_SOCKETHANDLER_H_
#define LIBFORC_NET_SOCKETHANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <mqueue.h>
#include "../collection/lfcQueue.h"
#include "../core/lfcObject.h"
#include "lfcSocketJobReader.h"
#include "lfcSocketJobWriter.h"
#include <stdio.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocketHandler);

// forward decl
typedef struct lfcSocket lfcSocket_t;
typedef struct lfcSocket_class lfcSocket_class_t;

typedef void (*fn_onReadrComplete_cb)(void *context, ssize_t read_len, char *buf, size_t buf_len);

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcSocketHandler { const struct lfcObject _;
    lfcSocketJobReader_t *cmd_pipe_readJob;

    lfcList_t *pending_jobs; // Aufträge (lesen oder schreiben) in der Warteschlange

    int epoll_fd;

    /**
     * Wird genutzt um z.B. thread_is_running zu setzen.
     */
    pthread_mutex_t thread_global_mutex;

    /**
     * Wird zur ewigen Schleife genutzt. Ist die Variable 0, wird er Thread beim nächsten Aufwachen verlassen.
     */
    int8_t thread_is_running;

    /**
     * Hier ist die ID unseres Threads
     */
    pthread_t thread;

    /**
     * Wird gesetzt, wenn der Thread ordnungsgemäß starten konnte
     */
    pthread_cond_t thread_is_startet_cond;

    /**
     * Wird der Thread gestartet schriebt er hier seinen Fehlerwert rein. Ist er 0, ist alles ok.
     * Ist er !=0, wird die Initialisierung abgebrochen.
     */
    int8_t thread_start_error_code;

    int thread_cmd_pipe[2]; // zur komm. mit dem Thread (watcher hinzufuegen, oder löschen, beenden, oder Senden)

    /**
     * Wird genutzt um in die pipe zu schreiben.
     */
    pthread_mutex_t cmd_pipe_write_mutex;
};

struct lfcSocketHandler_class { const struct lfcObject_class _;
    method_t read;
    method_t write;

    method_t killall;
    method_t killall_waitFor;

    method_t printSupportData;
};

struct lfcSocketHandler_methods {
    int (*read)(lfcSocketHandler_t *self, lfcSocketJobReader_t *job);
    int (*write)(lfcSocketHandler_t *self, lfcSocketJobWriter_t *job);

    pthread_mutex_t *(*killall)(lfcSocketHandler_t *self, int fd);
    int (*killall_waitFor)(lfcSocketHandler_t *self, int fd, long timeout_in_ms);

    int (*printSupportData)(lfcSocketHandler_t *self, FILE *fp);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Gibt die lfcSocketHandler Instanz zurück.
 */
lfcSocketHandler_t *lfcSocketHandler_ctor (void);

/**
 * Lesen von Daten über den Socket.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job ist der Lese-Job mit all seinen Beschreibungen
 * @return 0 .. ok, Lesen konnte eingequeued werden
 *          -EINVAL .. Parameter fehlerhaft
 */
int lfcSocketHandler_read (
    lfcSocketHandler_t *self,
    lfcSocketJobReader_t *job
);

/**
 * Schreiben von Daten über den Socket.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param job ist der Schreib-Job mit all seinen Beschreibungen
 * @return 0 .. ok, Lesen konnte eingequeued werden
 *          -EINVAL .. Parameter fehlerhaft
 */
int lfcSocketHandler_write (
    lfcSocketHandler_t *self,
    lfcSocketJobWriter_t *job
);

/**
 * Löschen des angegebenen FDs aus dem Handler.
 * Die Funktion gibt einen Mutex zurück, der freigegeben wird, sobald ALLE Jobs die auf dem fd lauschen gelöscht werden
 * konnten.
 * Der Aufrufer versucht dann ein <code>pthread_mutex_timedlock</code> mit einer individuellen timeout-Erwartung.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param fd zu entfernender FD
 * @return  !NULL .. ok, Mutex der freigegeben wird, sobald vom Thread alle Jobs gelöscht wurden sind
 *          NULL .. Fehler, (aber der FD wird auf jeden Fall rausgenommen, es gibt halt nur kein Mutex zur Bestätigung)
 */
pthread_mutex_t *lfcSocketHandler_killall (
    lfcSocketHandler_t *self,
    int fd
);

/**
 * Löschen des angegebenen FDs aus dem Handler.
 * Und warten auf Bestätigung des Löschens.
 *
 * @param self ist der Instanz-ptr vom socket client
 * @param fd zu entfernender FD
 * @param timeout_in_ms Timeout in milli seconds
 * @return  0 .. ok, Mutex der freigegeben wird, sobald vom Thread alle Jobs gelöscht wurden sind
 *          <0 .. Fehler
 */
int lfcSocketHandler_killall_waitFor (
    lfcSocketHandler_t *self,
    int fd,
    long timeout_in_ms
);

/**
 * Ausgabe aller relevanten Daten auf fp.
 *
 * @param self ist unsere Instanz
 * @param fp hierhin wird geschrieben
 * @return 0 .. ok
 *          <0 .. Fehler
 */
int lfcSocketHandler_printSupportData (
    lfcSocketHandler_t *self,
    FILE *fp
);


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_NET_SOCKETHANDLER_H_ ---*/
