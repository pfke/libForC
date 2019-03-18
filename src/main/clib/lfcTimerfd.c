#include "lfcTimerfd.h"

#include <bits/types.h>
#include <sys/timerfd.h>
#include <string.h>

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
) {
    struct itimerspec new_value;

    memset(&new_value, 0, sizeof(new_value));

    if (!initial_in_ms) {
        initial_in_ms = interval_in_ms;
    }

    __time_t initial_in_ms_sec = initial_in_ms / 1000;
    long int initial_in_ms_nsec = (initial_in_ms % 1000) * 1000 * 1000;
    new_value.it_value.tv_sec = initial_in_ms_sec;
    new_value.it_value.tv_nsec = initial_in_ms_nsec;

    __time_t interval_in_ms_sec = interval_in_ms / 1000;
    long int interval_in_ms_nsec = (interval_in_ms % 1000) * 1000 * 1000;
    new_value.it_interval.tv_sec = interval_in_ms_sec;
    new_value.it_interval.tv_nsec = interval_in_ms_nsec;

    return timerfd_settime(fd, 0, &new_value, NULL);
}

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
) {
    return lfc_timerfd_settime_interval(fd, initial_in_ms, 0);
}

/**
 * Diese Funktion desktiviert den Timer.
 *
 * @param fd ist der file descriptor des Timers
 * @return 0 .. success
 *          -! .. error (see errno)
 */
int lfc_timerfd_settime_disarm (
    int fd
) {
    struct itimerspec new_value;

    memset(&new_value, 0, sizeof(new_value));
    return timerfd_settime(fd, 0, &new_value, NULL);
}
