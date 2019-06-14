#include <stdio.h>
#include <tools/lfcOptParser.h>
#include <data/lfcStringOps.h>
#include <malloc.h>
#include <mem/lfcMemPool.h>
#include <net/lfcSocket.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocketJobReaderStream.h>
#include <threading/lfcThreadPool.h>
#include <threading/actor/lfcActorSystem.h>
#include <net/lfcSocketJobAcceptConn.h>

#define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))
#define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

void onReadComplete_cb(
    void *context,
    void *ident,
    ssize_t read_len,
    char *read_buf
) {
    fprintf(stderr, "%s@%d: read_buf='%s'\n", __func__,  __LINE__, read_buf);
}

void onAcceptConn_cb (
    lfcSocket_t *acceptSocket,
    lfcSocket_t *newSocket,
    void *context
) {
    fprintf(stderr, "%s@%d: \n", __func__, __LINE__);

    lfcSocket_read_job(
        newSocket,
        asInstanceOf(
            lfcSocketJob(),
            lfcSocketJobReaderStream_ctor(
                lfcSocket_getFd(newSocket),
                "1234",
                NULL,
                -1,
                100,
                onReadComplete_cb
            ))
    );

    //lfcSocket_read_async(
    //    newSocket,
    //    calloc(1, 100), 100,
    //    -1, 0,
    //    onReadComplete_cb
    //);
}

void runner_fn () {
    int pipe_fds_1[2];

    pipe(pipe_fds_1);

    void my_fn_onReadComplete_cb(void *context, void *ident, ssize_t read_len, char *read_buf) {
//        fprintf(stderr, "%s@%d: read='%s'\n", __func__,  __LINE__, read_buf);
    }
//    void my_fn_onWriteComplete_cb(void *context, void *ident, ssize_t written_len, const char *buf) {}

    lfcSocketHandler_t *tto = lfcSocketHandler_ctor();

    lfcSocket_t *reader_1 = lfcSocket_ctor(tto, pipe_fds_1[0]);
    lfcSocket_t *writer_1 = lfcSocket_ctor(tto, pipe_fds_1[1]);
    lfcSocket_read_job( reader_1, asInstanceOf(lfcSocketJobReader(), lfcSocketJobReaderStream_ctor(pipe_fds_1[0], NULL, NULL, 0, 100, my_fn_onReadComplete_cb)));
//    lfcSocket_read_job( reader_1, lfcSocketJobReader_ctor(pipe_fds_1[0], NULL, NULL, 0,      strlen(buf), my_fn_onReadComplete_cb));

    void *threaddie(void *arg) {
        delete(reader_1);

        return NULL;
    }

    pthread_t tinfo;

//    fprintf(stderr, "%s@%d: pipe_fds_1[0]=%d, pipe_fds_1[1]=%d\n", __func__,  __LINE__, pipe_fds_1[0], pipe_fds_1[1]);
    lfcSocket_write(writer_1, "before_1", strlen("before_1"), -1);
    lfcSocket_write(writer_1, "before_2", strlen("before_2"), -1);
    lfcSocket_write(writer_1, "before_3", strlen("before_3"), -1);
    lfcSocket_write(writer_1, "before_4", strlen("before_4"), -1);
    lfcSocket_write(writer_1, "before_5", strlen("before_5"), -1);
    lfcSocket_write(writer_1, "before_6", strlen("before_6"), -1);
    lfcSocket_write(writer_1, "before_7", strlen("before_7"), -1);
    lfcSocket_write(writer_1, "before_8", strlen("before_8"), -1);
    lfcSocket_write(writer_1, "before_9", strlen("before_9"), -1);
    lfcSocket_write(writer_1, "before_0", strlen("before_0"), -1);
    pthread_create(&tinfo, NULL, threaddie, "jkljkl");
    lfcSocket_write(writer_1, "after_1", strlen("after_1"), -1);
    lfcSocket_write(writer_1, "after_2", strlen("after_2"), -1);
    lfcSocket_write(writer_1, "after_3", strlen("after_3"), -1);
    lfcSocket_write(writer_1, "after_4", strlen("after_4"), -1);
    lfcSocket_write(writer_1, "after_5", strlen("after_5"), -1);
    lfcSocket_write(writer_1, "after_6", strlen("after_6"), -1);
    lfcSocket_write(writer_1, "after_7", strlen("after_7"), -1);
    lfcSocket_write(writer_1, "after_8", strlen("after_8"), -1);
    lfcSocket_write(writer_1, "after_9", strlen("after_9"), -1);
    lfcSocket_write(writer_1, "after_0", strlen("after_0"), -1);

//    fprintf(stderr, "%d@%s\n", __LINE__, __func__);
    sleep(1);

    delete(reader_1);

    sleep(5);

    delete(tto);
}

int main (
    int argc,
    char *argv[]
) {
    runner_fn();

    return 0;
}
