#include <net/lfcSocketJobWriter.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <asm/errno.h>
#include <core/lfcCore.h>

#define TEST_SUITE_NAME               spec_lfcSocketJobWriter_doWork

/**
 * --single=spec_lfcSocketJobWriter_doWork::write_complete_in_one_chunk
 */
Test(
    TEST_SUITE_NAME,
    write_complete_in_one_chunk
) {
    int context;
    int ident;
    int pipe_fds[2];

    char *buf = "hello";
    char *rec_buf;

    pipe(pipe_fds);

    rec_buf = calloc(1, strlen(buf));

    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(
        pipe_fds[1],
        &context, &ident,
        0,
        buf,
        strlen(buf),
        lambda(void, (void *res_context, void *res_ident, ssize_t res_written_len, const char *res_buf) {
            should_be_same_ptr_wText(res_context, &context, "callback-context stimmt nicht");
            should_be_same_ptr_wText(res_ident, &ident, "callback-ident stimmt nicht");
            should_be_same_int_wText(res_written_len, strlen(buf), "Anzahl gelesender Bytes stimmt nicht");
            should_be_same_ptr_wText(res_buf, buf, "Buffer stimmt nicht");
        })
    );

    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");
    read(pipe_fds[0], rec_buf, strlen(buf));
    should_be_same_array_wText(rec_buf, buf, strlen(buf), "Daten stimmen nicht");

    delete(tto);
    free(rec_buf);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(
    TEST_SUITE_NAME,
    write_on_closed_fd
) {
    int context;
    int ident;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(
        pipe_fds[1],
        &context, &ident,
        0,
        buf,
        strlen(buf),
        lambda(void, (void *res_context, void *res_ident, ssize_t res_written_len, const char *res_buf) {
            should_be_same_ptr_wText(res_context, &context, "callback-context stimmt nicht");
            should_be_same_ptr_wText(res_ident, &ident, "callback-ident stimmt nicht");
            should_be_same_int_wText(res_written_len, -EBADF, "Anzahl gelesender Bytes stimmt nicht");
            should_be_same_ptr_wText(res_buf, buf, "Buffer stimmt nicht");
        })
    );

    close(pipe_fds[0]);
    close(pipe_fds[1]);

    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), -EBADF, "lfcSocketJob_yourFDIsReady hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    delete(tto);
}
