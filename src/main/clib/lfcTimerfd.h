#ifndef LIBFORC_CLIB_TIMERFD_H
#define LIBFORC_CLIB_TIMERFD_H

/**
 * Diese Funktion setzt einen Intervall-Timer auf.
 *
 * @param fd ist der file descriptor des Timers
 * @param initial_in_ms ist der initiale timer-Wert (ist er 0, wird das interval genommen)
 * @param interval_in_ms Interval in ms
 * @return 0 .. success
 *          -! .. error (see errno)
 */
int lfc_timerfd_settime_interval (
    int fd,
    long initial_in_ms,
    long interval_in_ms
);

/**
 * Diese Funktion setzt einen OneShot-Timer auf.
 *
 * @param fd ist der file descriptor des Timers
 * @param initial_in_ms ist der initiale timer-Wert
 * @return 0 .. success
 *          -! .. error (see errno)
 */
int lfc_timerfd_settime_oneShot (
    int fd,
    long initial_in_ms
);

/**
 * Diese Funktion desktiviert den Timer.
 *
 * @param fd ist der file descriptor des Timers
 * @return 0 .. success
 *          -! .. error (see errno)
 */
int lfc_timerfd_settime_disarm (
    int fd
);

#endif //LIBFORC_CLIB_TIMERFD_H
