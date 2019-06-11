#include <stdio.h>
#include <tools/lfcOptParser.h>
#include <data/lfcStringOps.h>
#include <malloc.h>
#include <mem/lfcMemPool.h>
#include <net/lfcSocket.h>
#include <net/lfcSocketHandler.h>
#include <threading/lfcThreadPool.h>
#include <threading/actor/lfcActorSystem.h>
#include <net/lfcSocketJobAcceptConn.h>

#define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))
#define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

void onAcceptConn_cb (
    lfcSocket_t *acceptSocket,
    lfcSocket_t *newSocket,
    void *context
) {
    fprintf(stderr, "%s@%d: \n", __func__, __LINE__);
}

void runner_fn () {
    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *server = lfcSocket_listenFor_tcpStream(
        socketHandler,
        "localhost", "1234",
        "1234", onAcceptConn_cb
    );

    lfcSocket_t *server2 = lfcSocket_listenFor_unixDomain_asSeqPacket(
        socketHandler,
        "localhost",
        "1234", onAcceptConn_cb
    );

//    lfcSocket_listen(server, "1234", onAcceptConn_cb);

//    lfcSocket_
//
//    lfcSocketJobAcceptConn_t *acceptJob = lfcSocketJobAcceptConn_ctor();

    fprintf(stderr, "%s@%d: \n", __func__, __LINE__);

//    lfcActorSystem_isRemoteActorName_viaTCP("lfcAA.tcp://m-m_8950-si@192.168.178.1:123");
//    lfcActorSystem_isRemoteActorName_viaTCP("lfcAA.tcp://m-m_8950-si@192.168.178.1:1231");
    sleep(120);
}

int main (
    int argc,
    char *argv[]
) {
    runner_fn();

    return 0;
}
