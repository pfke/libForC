#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>

// --pattern "spec_lfcSocket_read_async/*"
#define TEST_SUITE_NAME               spec_lfcSocket_read_async

Test(
    TEST_SUITE_NAME,
    oneShot_noTimeout
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    char *reader_buffer = calloc(1, strlen(buf));
    ssize_t result;
    bool read_wait_for = false;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *reader = lfcSocket_ctor(socketHandler, pipe_fds[0]);

    write(pipe_fds[1], buf, strlen(buf));
    result = lfcSocket_read_async(
        reader, reader_buffer, strlen(buf), -1, 1,
        lambda(void, (void *res_context, void *res_ident, ssize_t res_read_len, char *res_buf) {
            should_be_null(res_context);
            should_be_null(res_ident);
            should_be_same_int_wText(res_read_len, strlen(buf), "Anzahl gelesender Bytes stimmt nicht");
            should_be_same_array_wText(res_buf, buf, strlen(buf), "Daten stimmen nicht");

            read_wait_for = true;
        })
    );

    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &read_wait_for);
    should_be_same_int_wText(result, 0, "Return value stimmt nicht");
    should_be_same_array_wText(reader_buffer, buf, strlen(buf), "Daten stimmen nicht");

    delete(reader);
    delete(socketHandler);
}
