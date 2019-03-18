#include <ipc/lfcMQueueReceiver.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>

// --pattern "spec_lfcSocket_write_async/*"
#define TEST_SUITE_NAME               spec_lfcSocket_write_async

Test(
    TEST_SUITE_NAME,
    timeout_minus1__wData
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    char *reader_buffer = calloc(1, strlen(buf));
    ssize_t result;
    bool write_wait_for = false;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write_async(
        socket, buf, strlen(buf), -1,
        lambda(void, (void *res_context, void *res_ident, ssize_t res_written_len, const char *res_buf) {
            should_be_null(res_context);
            should_be_null(res_ident);
            should_be_same_int_wText(res_written_len, strlen(buf), "Anzahl gelesender Bytes stimmt nicht");
            should_be_same_ptr_wText(res_buf, buf, "Buffer stimmt nicht");

            write_wait_for = true;
        })
    );
    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &write_wait_for);
    read(pipe_fds[0], reader_buffer, strlen(buf));

    should_be_same_int_wText(result, 0, "Return value stimmt nicht");
    should_be_same_array_wText(reader_buffer, buf, strlen(buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}
