#include <ipc/lfcMQueueReceiver.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>

#define TEST_SUITE_NAME               spec_lfcSocket_dtor

/**
 * --single=spec_lfcSocket_dtor::just_flow
 */
Test(
    TEST_SUITE_NAME,
    just_flow
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    char *reader_buffer = calloc(1, strlen(buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *reader = lfcSocket_ctor(socketHandler, pipe_fds[0]);

    write(pipe_fds[1], buf, strlen(buf));
    result = lfcSocket_read(reader, reader_buffer, strlen(buf), -1);

    should_be_same_int_wText(result, strlen(buf), "Anzahl gelesender Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, buf, strlen(buf), "Daten stimmen nicht");

    delete(reader);
    delete(socketHandler);
}
