#include <stdio.h>
#include <tools/lfcOptParser.h>
#include <data/lfcStringOps.h>
#include <malloc.h>
#include <mem/lfcMemPool.h>
#include <threading/lfcThreadPool.h>
#include <threading/actor/lfcActorSystem.h>

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
    lfcActorSystem_isRemoteActorName_viaTCP("lfcAA.tcp://m-m_8950-si@192.168.178.1:123");
    lfcActorSystem_isRemoteActorName_viaTCP("lfcAA.tcp://m-m_8950-si@192.168.178.1:1231");
}

int main (
    int argc,
    char *argv[]
) {
    runner_fn();

    return 0;
}
