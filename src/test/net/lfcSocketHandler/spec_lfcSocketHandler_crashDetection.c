#include <ipc/lfcMQueueReceiver.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketJobWriter.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>
#include <net/lfcSocketJobReaderStream.h>

#define TEST_SUITE_NAME               spec_lfcSocketHandler_crashDetection

Test(
    TEST_SUITE_NAME,
    ret_ok
) {
    int pipe_fds_1[2];

    pipe(pipe_fds_1);

    void my_fn_onReadComplete_cb(void *context isAnUnused_param, void *ident isAnUnused_param, ssize_t read_len isAnUnused_param, char *read_buf isAnUnused_param) {
//        fprintf(stderr, "%s@%d: read='%s'\n", __func__,  __LINE__, read_buf);
    }
//    void my_fn_onWriteComplete_cb(void *context, void *ident, ssize_t written_len, const char *buf) {}

    lfcSocketHandler_t *tto = lfcSocketHandler_ctor();

    lfcSocket_t *reader_1 = lfcSocket_ctor(tto, pipe_fds_1[0]);
    lfcSocket_t *writer_1 = lfcSocket_ctor(tto, pipe_fds_1[1]);
    lfcSocket_read_job( reader_1, asInstanceOf(lfcSocketJobReader(), lfcSocketJobReaderStream_ctor(pipe_fds_1[0], NULL, NULL, 0, 100, my_fn_onReadComplete_cb)));
//    lfcSocket_read_job( reader_1, lfcSocketJobReader_ctor(pipe_fds_1[0], NULL, NULL, 0,      strlen(buf), my_fn_onReadComplete_cb));

    void *threaddie(void *arg isAnUnused_param) {
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
