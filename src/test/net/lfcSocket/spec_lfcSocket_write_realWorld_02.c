#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocket.h>
#include <sys/types.h>
#include <sys/socket.h>

/**
 * Eine Seite auf der permanent gelesen wird und abundzu geschrieben.
 * epoll_ctl(MOD, ...) wird hier getestet
 */
// --pattern "spec_lfcSocket_write_realWorld_02/*"
#define TEST_SUITE_NAME               spec_lfcSocket_write_realWorld_02

/**
 * --single=spec_lfcSocket_write_realWorld_02::packet_01
 */
Test(
    TEST_SUITE_NAME,
    packet_01
) {
    int pipe_fds[2];
    char in_buf[] = { 0x0, 0x0, 0x0, 0x17 };
    char *reader_buffer = calloc(1, sizeof(in_buf));
    bool sock0_read_wait_for = false;
    bool sock1_read_wait_for = false;
    size_t sock0_read_counter = 0;
    size_t sock1_read_counter = 0;

    socketpair(PF_LOCAL, SOCK_STREAM, 0, pipe_fds);

//    fprintf(stderr, "%s@%d: pipe_fds[0]=%u, pipe_fds[1]=%u\n", __func__, __LINE__, pipe_fds[0], pipe_fds[1]);

    lfcSocketHandler_t *socketHandler = lfcSocketHandler_ctor();
    lfcSocket_t *socket_0 = lfcSocket_ctor(socketHandler, pipe_fds[0]);
    lfcSocket_read_async(
        socket_0, reader_buffer, sizeof(in_buf), -1, 0,
        lambda(void, (void *res_context isAnUnused_param, void *res_ident isAnUnused_param, ssize_t res_read_len isAnUnused_param, char *res_buf isAnUnused_param) {
//            fprintf(stderr, "[0] %d@%s: res_read_len=%zd (counter=%zu)\n", __LINE__, __func__, res_read_len, sock0_read_counter);
            sock0_read_counter++;

            if (sock0_read_counter >= 2) {
                sock0_read_wait_for = true;
            }
        })
    );

    lfcSocket_t *socket_1 = lfcSocket_ctor(socketHandler, pipe_fds[1]);
    lfcSocket_read_async(
        socket_1, reader_buffer, sizeof(in_buf), -1, 0,
        lambda(void, (void *res_context isAnUnused_param, void *res_ident isAnUnused_param, ssize_t res_read_len isAnUnused_param, char *res_buf isAnUnused_param) {
//            fprintf(stderr, "[1] %d@%s: res_read_len=%zd (counter=%zu)\n", __LINE__, __func__, res_read_len, sock1_read_counter);
            sock1_read_counter++;

            if (sock1_read_counter >= 2) {
                sock1_read_wait_for = true;
            }
        })
    );

    lfcSocket_write(socket_1, in_buf, sizeof(in_buf), -1);
    lfcSocket_write(socket_0, in_buf, sizeof(in_buf), -1);
    lfcSocket_write(socket_1, in_buf, sizeof(in_buf), -1);
    lfcSocket_write(socket_0, in_buf, sizeof(in_buf), -1);

    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &sock0_read_wait_for);
    lfcCriterionHelper_waitUntil_varIsTrue(1000 * 1000000, 100 * 10000, &sock1_read_wait_for);

    delete(socket_0);
    delete(socket_1);
    delete(socketHandler);

    close(pipe_fds[0]);
    close(pipe_fds[1]);
}
