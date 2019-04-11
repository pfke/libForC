/*
 * Copyright (c) 2016, Mathias Brossard <mathias@brossard.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIBFORC_THREADING_LFCTHREADPOOL_H
#define LIBFORC_THREADING_LFCTHREADPOOL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file lfcThreadPool.h
 * @brief Threadpool Header File
 */

/**
* Increase this constants at your own risk
* Large values might slow down your system
*/
#define MAX_THREADS 64
#define MAX_QUEUE 65536

typedef struct lfcThreadPool_ lfcThreadPool_t;

typedef enum {
    threadpool_invalid        = -1,
    threadpool_lock_failure   = -2,
    threadpool_queue_full     = -3,
    threadpool_shutdown       = -4,
    threadpool_thread_failure = -5
} lfcThreadPool_error_t;

typedef enum {
    threadpool_graceful       = 1
} lfcThreadPool_destroy_flags_t;

/**
 * @function threadpool_add
 * @brief add a new task in the queue of a thread pool
 * @param pool      Thread pool to which add the task.
 * @param worker_fn Pointer to the function that will perform the task.
 * @param argument  Argument to be passed to the function.
 * @param flags     Unused parameter.
 * @return 0 if all goes well, negative values in case of error (@see
 * threadpool_error_t for codes).
 */
int lfcThreadPool_addWorker(
    lfcThreadPool_t *pool,
    void (*worker_fn)(void *),
    void *arg,
    int flags
);

/**
 * @function threadpool_create
 * @brief Creates a threadpool_t object.
 * @param thread_count Number of worker threads.
 * @param queue_size   Size of the queue.
 * @param flags        Unused parameter.
 * @return a newly created thread pool or NULL
 */
lfcThreadPool_t *lfcThreadPool_create (
    int thread_count,
    int queue_size,
    int flags
);

/**
 * @function threadpool_destroy
 * @brief Stops and destroys a thread pool.
 * @param pool  Thread pool to destroy.
 * @param flags Flags for shutdown
 *
 * Known values for flags are 0 (default) and threadpool_graceful in
 * which case the thread pool doesn't accept any new tasks but
 * processes all pending tasks before shutdown.
 */
int lfcThreadPool_destroy(
    lfcThreadPool_t *pool,
    int flags
);

#ifdef __cplusplus
}
#endif

#endif //LIBFORC_THREADING_LFCTHREADPOOL_H