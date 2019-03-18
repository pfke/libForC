#include <ipc/lfcMQueueReceiver.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>

// --pattern "spec_lfcSocket_write_realWorld_01/*"
#define TEST_SUITE_NAME               spec_lfcSocket_write_realWorld_01

Test(
    TEST_SUITE_NAME,
    packet_01
) {
    int pipe_fds[2];
    char in_buf[] = { 0x0, 0x0, 0x0, 0x17 };
    char *reader_buffer = calloc(1, sizeof(in_buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, in_buf, sizeof(in_buf), -1);
    read(pipe_fds[0], reader_buffer, sizeof(in_buf));

    should_be_same_int_wText(result, sizeof(in_buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, (char *)in_buf, sizeof(in_buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}

Test(
    TEST_SUITE_NAME,
    packet_02
) {
    int pipe_fds[2];
    char in_buf[] = {
        0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00
    };
    char *reader_buffer = calloc(1, sizeof(in_buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, in_buf, sizeof(in_buf), -1);
    read(pipe_fds[0], reader_buffer, sizeof(in_buf));

    should_be_same_int_wText(result, sizeof(in_buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, (char *)in_buf, sizeof(in_buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}

Test(
    TEST_SUITE_NAME,
    packet_03
) {
    int pipe_fds[2];
    char in_buf[] = { 0x0, 0x0, 0x0, 0x0a };
    char *reader_buffer = calloc(1, sizeof(in_buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, in_buf, sizeof(in_buf), -1);
    read(pipe_fds[0], reader_buffer, sizeof(in_buf));

    should_be_same_int_wText(result, sizeof(in_buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, (char *)in_buf, sizeof(in_buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}

Test(
    TEST_SUITE_NAME,
    packet_04
) {
    int pipe_fds[2];
    char in_buf[] = { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
    char *reader_buffer = calloc(1, sizeof(in_buf));
    ssize_t result;

    pipe(pipe_fds);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket = lfcSocket_ctor(socketHandler, pipe_fds[1]);

    result = lfcSocket_write(socket, in_buf, sizeof(in_buf), -1);
    read(pipe_fds[0], reader_buffer, sizeof(in_buf));

    should_be_same_int_wText(result, sizeof(in_buf), "Anzahl geschriebener Bytes stimmt nicht");
    should_be_same_array_wText(reader_buffer, (char *)in_buf, sizeof(in_buf), "Daten stimmen nicht");

    delete(socket);
    delete(socketHandler);
}
