#include <ipc/lfcMQueueReceiver.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>

// --pattern "spec_lfcSocketHandler_read/*"
#define TEST_SUITE_NAME               spec_lfcSocketHandler_read

/**
 * --single=spec_lfcSocketHandler_read::ret_ok
 */
Test(
    TEST_SUITE_NAME,
    timeout_0
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    bool read_wait_for = false;
    bool write_wait_for = false;

    pipe(pipe_fds);

    lfcSocketHandler_t *tto = lfcSocketHandler_ctor();

    lfcSocketHandler_read(
        tto,
        lfcSocketJobReader_ctor(
            pipe_fds[0],
            NULL, NULL,
            0,
            strlen(buf),
            lambda(void, (void *res_context, void *res_ident, ssize_t res_read_len, char *res_buf) {
//                fprintf(stderr, "[r] %d@%s: buf='%s'\n", __LINE__, __func__, res_buf);
                should_be_same_int_wText(res_read_len, strlen(buf), "[r] Anzahl gelesender Bytes stimmt nicht");
                should_be_same_array_wText(res_buf, buf, strlen(buf), "[r] Daten stimmen nicht");

                read_wait_for = true;
            })
        )
    );

    lfcSocketHandler_write(
        tto,
        lfcSocketJobWriter_ctor(
            pipe_fds[1],
            NULL, NULL,
            0,
            buf, strlen(buf),
            lambda(void, (void *res_context, void *res_ident, ssize_t res_written_len, const char *res_buf) {
//                fprintf(stderr, "[w] %d@%s: buf='%s'\n", __LINE__, __func__, res_buf);
                should_be_same_int_wText(res_written_len, strlen(buf), "[w] Anzahl gelesender Bytes stimmt nicht");
                should_be_same_ptr_wText(res_buf, buf, "[w] Daten stimmen nicht");

                write_wait_for = true;
            })
        )
    );

    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &read_wait_for);
    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &write_wait_for);

//    fprintf(stderr, "%d@%s\n", __LINE__, __func__);

    free(tto);
}

Test(
    TEST_SUITE_NAME,
    timeout_1
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    bool read_wait_for = false;
    bool write_wait_for = false;

    pipe(pipe_fds);

    lfcSocketHandler_t *tto = lfcSocketHandler_ctor();

    lfcSocketHandler_read(
        tto,
        lfcSocketJobReader_ctor(
            pipe_fds[0],
            NULL, NULL,
            1,
            strlen(buf),
            lambda(void, (void *res_context, void *res_ident, ssize_t res_read_len, char *res_buf) {
//                fprintf(stderr, "[r] %d@%s: buf='%s'\n", __LINE__, __func__, res_buf);
                should_be_same_int_wText(res_read_len, strlen(buf), "[r] Anzahl gelesender Bytes stimmt nicht");
                should_be_same_array_wText(res_buf, buf, strlen(buf), "[r] Daten stimmen nicht");

                read_wait_for = true;
            })
        )
    );

    lfcSocketHandler_write(
        tto,
        lfcSocketJobWriter_ctor(
            pipe_fds[1],
            NULL, NULL,
            0,
            buf, strlen(buf),
            lambda(void, (void *res_context, void *res_ident, ssize_t res_written_len, const char *res_buf) {
//                fprintf(stderr, "[w] %d@%s: buf='%s'\n", __LINE__, __func__, res_buf);
                should_be_same_int_wText(res_written_len, strlen(buf), "[w] Anzahl gelesender Bytes stimmt nicht");
                should_be_same_ptr_wText(res_buf, buf, "[w] Daten stimmen nicht");

                write_wait_for = true;
            })
        )
    );

    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &read_wait_for);
    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &write_wait_for);

//    fprintf(stderr, "%d@%s\n", __LINE__, __func__);

    delete(tto);
}
