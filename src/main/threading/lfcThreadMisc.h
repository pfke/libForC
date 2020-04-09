#ifndef LIBFORC_THREADING_THREADMISC_H_
#define LIBFORC_THREADING_THREADMISC_H_

#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Make a getter function with locking:
 *
 * input:  MAKE_FN_GETTER(int8)
 * output: static inline int8_t get_int8_wLocking(int8_t *var, pthread_mutex_t *mutex) {
 *             int8_t ret = 0;
 *             pthread_mutex_lock(mutex);
 *             ret = *var;
 *             pthread_mutex_unlock(mutex);
 *             return ret;
 *         }
 */
#define MAKE_FN_GETTER(x) static inline x##_t get_##x##_wLocking (x##_t *var, pthread_mutex_t *mutex) { \
                                  x##_t ret = 0;                                                            \
                                  pthread_mutex_lock(mutex);                                                \
                                  ret = *var;                                                               \
                                  pthread_mutex_unlock(mutex);                                              \
                                  return ret;                                                               \
                              }

/**
 * Make a setter function with locking:
 *
 * input:  MAKE_FN_SETTER(int8)
 * output: static inline int set_int8_wLocking(int8_t *var, pthread_mutex_t *mutex, int8_t value) {
 *             int result = 0;
 *             pthread_mutex_lock(mutex);
 *             result = *var == value;
 *             *var = value;
 *             pthread_mutex_unlock(mutex);
 *             return result;
 *         }
 *
 * @return 
 *      0 .. var was changed to the given value
 *      1 .. var is already changed
 */
#define MAKE_FN_SETTER(x) static inline int set_##x##_wLocking (x##_t *var, pthread_mutex_t *mutex, x##_t value) { \
                                  int result = 0;                                                                       \
                                  pthread_mutex_lock(mutex);                                                            \
                                  result = *var == value;                                                               \
                                  *var = value;                                                                         \
                                  pthread_mutex_unlock(mutex);                                                          \
                                  return result;                                                                        \
                              }

/**
 * Schreiben eines mutex-geschützten Wertes und setzen eines Signals.
 *
 * input: MAKE_FN_SENDSIGNAL(int8)
 * output static inline int set_int8_andSignal (
 *            int8_t *var,
 *            int8_t value,
 *            pthread_mutex_t *mutex,
 *            pthread_cond_t *cond
 *        ) {
 *            if (0 == pthread_mutex_lock(mutex)) {
 *                *var = value;
 *                pthread_cond_signal(cond);
 *
 *                pthread_mutex_unlock(mutex);
 *                return 0;
 *            }
 *            return -1;
 *        }
 */
#define MAKE_FN_SENDSIGNAL(x) static inline int set_##x##_sendSignal (x##_t *var, x##_t value, pthread_mutex_t *mutex, pthread_cond_t *cond) { \
                                  if (0 == pthread_mutex_lock(mutex)) {                                                     \
                                      *var = value;                                                                         \
                                      pthread_cond_signal(cond);                                                            \
                                      pthread_mutex_unlock(mutex);                                                          \
                                      return 0;                                                                             \
                                  }                                                                                         \
                                  return -1;                                                                                \
                              }


/**
 * Lesen eines mutex-geschützten Wertes NACH Empfang eines Signals.
 *
 * input: MAKE_FN_READSIGNAL(int8)
 * output static inline int8_t get_int8_awaitSignal (
 *            int8_t *var,
 *            pthread_mutex_t *mutex,
 *            pthread_cond_t *cond
 *        ) {
 *            int8_t result = 0;
 *            if (0 == pthread_mutex_lock(mutex)) {
 *                pthread_cond_signal(cond);
 *                result = *var;
 *                pthread_mutex_unlock(mutex);
 *            }
 *            return result;
 *        }
 */
#define MAKE_FN_READSIGNAL(x) static inline x##_t get_##x##_awaitSignal (x##_t *var, pthread_mutex_t *mutex, pthread_cond_t *cond) { \
                                  x##_t result = 0;                                                                         \
                                  if (0 == pthread_mutex_lock(mutex)) {                                                     \
                                      if(!(*var)) {                                                                         \
                                          pthread_cond_wait(cond, mutex);                                                   \
                                      }                                                                                     \
                                      result = *var;                                                                        \
                                      pthread_mutex_unlock(mutex);                                                          \
                                  }                                                                                         \
                                  return result;                                                                            \
                              }

/**
 * Lesen eines mutex-geschützten Wertes NACH Empfang eines Signals (timed).
 *
 * input: MAKE_FN_READSIGNAL(int8)
 * output static inline int8_t get_int8_awaitTimedSignal (
 *            int8_t *var,
 *            pthread_mutex_t *mutex,
 *            pthread_cond_t *cond,
 *            timeout_in_ms
 *        ) {
 *            int8_t result = 0;
 *            if (0 == pthread_mutex_lock(mutex)) {
 *                pthread_cond_signal(cond);
 *                result = *var;
 *                pthread_mutex_unlock(mutex);
 *            }
 *            return result;
 *        }
 */
#define MAKE_FN_READTIMEDSIGNAL(x) static inline x##_t get_##x##_awaitTimedSignal (x##_t *var, pthread_mutex_t *mutex, pthread_cond_t *cond, unsigned int timeout_in_ms) { \
                                  x##_t result = 0;                                                                         \
                                  struct timespec timeToWait;                                                               \
                                                                                                                            \
                                  if (0 == pthread_mutex_lock(mutex)) {                                                     \
                                      if(!(*var)) {                                                                         \
                                          if (clock_gettime(CLOCK_REALTIME, &timeToWait) == 0) {                            \
                                              timeToWait.tv_sec += (timeout_in_ms / 1000);                                  \
                                              timeToWait.tv_nsec += ((timeout_in_ms % 1000) * 1000 * 1000);                 \
                                              pthread_cond_timedwait(cond, mutex, &timeToWait);                             \
                                          } else {                                                                          \
                                              pthread_cond_wait(cond, mutex);                                               \
                                          }                                                                                 \
                                          result = *var;                                                                    \
                                      }                                                                                     \
                                      pthread_mutex_unlock(mutex);                                                          \
                                      }                                                                                     \
                                  return result;                                                                            \
                              }

/**
 * Make a setter function with locking:
 *
 * input:  MAKE_FN_POSTINC(int8)
 * output: static inline int8 pinc_int8_wLocking(int8_t *var, pthread_mutex_t *mutex) {
 *             int8_t result;
 *             pthread_mutex_lock(mutex);
 *             result = *var;
 *             *var = *var + 1;
 *             pthread_mutex_unlock(mutex);
 *
 *             return result;
 *         }
 */
#define MAKE_FN_POSTINC(x) static inline x##_t postinc_##x##_wLocking (x##_t *var, pthread_mutex_t *mutex) {        \
                                  x##_t result;                                                                         \
                                  pthread_mutex_lock(mutex);                                                            \
                                  result = *var;                                                                        \
                                  *var = *var + 1;                                                                      \
                                  pthread_mutex_unlock(mutex);                                                          \
                                  return result;                                                                        \
                              }

#define MAKE_LOCK_FN(x) \
    MAKE_FN_GETTER(x)       \
    MAKE_FN_SETTER(x)       \
    MAKE_FN_POSTINC(x)      \
    MAKE_FN_SENDSIGNAL(x)   \
    MAKE_FN_READSIGNAL(x)   \
    MAKE_FN_READTIMEDSIGNAL(x)

MAKE_LOCK_FN(int8);
MAKE_LOCK_FN(int16);
MAKE_LOCK_FN(int32);
MAKE_LOCK_FN(uint8);
MAKE_LOCK_FN(uint16);
MAKE_LOCK_FN(uint32);

/**
 * Diese Funktion liefert den Namen des aktuellen Thread zurueck.
 *
 * @param buf hierein wird der Name geschrieben. MUSS mindestens 16 byte gross sein.
 */
int lfcThreadMisc_getName (char *buf);

/**
 * Diese Funktion setzt den Thread-Namen.
 * Max. 20 Zeichen.
 *
 * @param fmt Format string
 * @param ... var args (used by fmt)
 * @return 0 .. hat funktioniert
 *         1 .. Fehler
 */
#define LFC_THREAD_MAXTHREADNAMELENGTH      16
int lfcThreadMisc_setName (
    const char *fmt,
    ...
);
int lfcThreadMisc_setName_valist (
    const char *fmt,
    va_list ap
);

#endif //LIBFORC_NET_SOCKETCLIENT_H_
