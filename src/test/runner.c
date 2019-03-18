#include <stdio.h>
#include <tools/lfcOptParser.h>
#include <data/lfcStringOps.h>
#include <malloc.h>
#include <mem/lfcMemPool.h>
#include <thread/lfcThreadPool.h>

#define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))
#define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

void worker (
    void *arg
) {
    fprintf(stderr, "%s@%d: %d\n", __func__,  __LINE__, (int)arg);
}

void runner_fn () {
    lfcThreadPool_t *tp = lfcThreadPool_create(5, 10, 0);

    lfcThreadPool_addWorker(tp, worker, (void *) 1, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 2, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 3, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 4, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 5, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 6, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 7, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 8, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 9, 0);
    lfcThreadPool_addWorker(tp, worker, (void *) 0, 0);

    lfcThreadPool_destroy(tp, threadpool_graceful);
}

int main (
    int argc,
    char *argv[]
) {
    runner_fn();

    return 0;
}
