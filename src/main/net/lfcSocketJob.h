#ifndef LIBFORC_NET_SOCKETJOB_H_
#define LIBFORC_NET_SOCKETJOB_H_

#include <stdint.h>
#include <stdbool.h>
//#include <unitypes.h>
#include <pthread.h>
#include "../core/lfcObject.h"
#include "lfcSocketJob.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcSocketJob)


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcSocketJob { const struct lfcObject _;
    int fd;
    void *context;
    void *ident;
    int timeout_in_s;

    unsigned int repeat;
    unsigned int current_run;

    bool enabled;

    pthread_mutex_t mutex;
};

struct lfcSocketJob_class { const struct lfcObject_class _;
    method_t enable;
    method_t disable;
    method_t isEnabled;

    method_t getFD;

    abstract method_t doWork; // nicht implementiert, MUSS von der ableitenden Klasse implementiert werden
    abstract method_t reset;
};

struct lfcSocketJob_methods {
    bool (*enable)(lfcSocketJob_t *self);
    bool (*disable)(lfcSocketJob_t *self);
    bool (*isEnabled)(lfcSocketJob_t *self);

    int (*getFD)(lfcSocketJob_t *self);

    int (*doWork)(lfcSocketJob_t *self);
    bool (*reset)(lfcSocketJob_t *self);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Aktiviert diesen Job, d.h. er wird beim Aufruf von <code>yourFDIsReady</code> oder <code>doWork</code> arbeiten.
 *
 * @param self ist die Instanz
 * @return true .. konnte aktiviert werden
 *          false .. konnte nicht aktiviert werden
 */
bool lfcSocketJob_enable (
    lfcSocketJob_t *self
);

/**
 * Deaktiviert diesen Job, d.h. er wird beim Aufruf von <code>yourFDIsReady</code> oder <code>doWork</code> nicht arbeiten.
 *
 * @param self ist die Instanz
 * @return true .. konnte deaktiviert werden
 *          false .. konnte nicht deaktiviert werden
 */
bool lfcSocketJob_disable (
    lfcSocketJob_t *self
);

/**
 * Gibt true zurück, wenn die4ser Job aktiviert ist.
 *
 * @param self ist die Instanz
 * @return true .. ist aktiviert
 *          false .. ist deaktiviert
 */
bool lfcSocketJob_isEnabled (
    lfcSocketJob_t *self
);

/**
 * Gibt den fd des Jobs zurueck.
 *
 * @param self Instanze-ptr
 * @return der fd
 */
int lfcSocketJob_getFD (
    lfcSocketJob_t *self
);

/**
 * fd is ready, dann mal los...
 *
 * @param self Instanze-ptr
 * @param fd file descriptor (zur Prüfung genutz, ob der mit dem gespeicherten uebereinstimmt)
 * @return  0 .. keine Fehler, Job ist noch nicht fertig
 *          1 .. der Job ist fertig und kann gelöscht werden
 *          -EINVAL .. Parameter ungültig
 */
int lfcSocketJob_doWork (
    lfcSocketJob_t *self
);

/**
 * Diese Methode setzt den Job zurück auf die Anffangsparameter. D.h. ein Reader liest beim nächsten AUfruf und geht von
 * bisher 0 empfangenen Daten aus.
 *
 * @param self ist die Instanz
 * @return true .. Reset hat funktioniert
 *          false .. Reset hat nicht funktioniert
 */
bool lfcSocketJob_reset (
    lfcSocketJob_t *self
);


#endif //LIBFORC_NET_SOCKETJOB_H_
