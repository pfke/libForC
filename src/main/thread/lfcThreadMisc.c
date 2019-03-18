#include "lfcThreadMisc.h"
#include <stdarg.h>
#include <sys/prctl.h>

/**
 * Diese Funktion liefert den Namen des aktuellen Thread zurueck.
 *
 * @param buf hierein wird der Name geschrieben. MUSS mindestens 16 byte gross sein.
 */
int lfcThreadMisc_getName (
    char *buf
) {
    return prctl(PR_GET_NAME, buf, 0, 0, 0);
}

/**
 * Diese Funktion setzt den Thread-Namen.
 *
 * @param fmt Format string
 * @param ... var args (used by fmt)
 * @return 0 .. hat funktioniert
 *         1 .. Fehler
 */
int lfcThreadMisc_setName (
    const char *fmt,
    ...
) {
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = lfcThreadMisc_setName_valist(fmt, ap);
    va_end(ap);

    return ret;
}

int lfcThreadMisc_setName_valist (
    const char *fmt,
    va_list ap
) {
    char buf[LFC_THREAD_MAXTHREADNAMELENGTH + 1];

    vsnprintf(buf, sizeof(buf), fmt, ap);
    return prctl(PR_SET_NAME, buf, 0, 0, 0);
}
