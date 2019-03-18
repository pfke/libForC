#include <ipc/lfcMQueueReceiver.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>

// --pattern "spec_lfcSocket_write/*"
#define TEST_SUITE_NAME               spec_lfcSocket_write

/**
 * --single=spec_lfcSocket_write::timeout_minus1__wData
 */
Test(
    TEST_SUITE_NAME,
    timeout_minus1__wData
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    char *reader_buffer = calloc(1, strlen(buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, buf, strlen(buf), -1);
    read(pipe_fds[0], reader_buffer, strlen(buf));

    should_be_same_int_wText(result, strlen(buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, buf, strlen(buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}

/**
 * --single=spec_lfcSocket_write::timeout_10__wData
 */
Test(
    TEST_SUITE_NAME,
    timeout_10__wData
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    char *reader_buffer = calloc(1, strlen(buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, buf, strlen(buf), 10);
    read(pipe_fds[0], reader_buffer, strlen(buf));

    should_be_same_int_wText(result, strlen(buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, buf, strlen(buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}

Test(
    TEST_SUITE_NAME,
    timeout_5__wData
) {
    int pipe_fds[2];
    char *buf = "hohei ";
    char *reader_buffer = calloc(1, strlen(buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, buf, strlen(buf), 5);
    read(pipe_fds[0], reader_buffer, strlen(buf));

    should_be_same_int_wText(result, strlen(buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, buf, strlen(buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}
