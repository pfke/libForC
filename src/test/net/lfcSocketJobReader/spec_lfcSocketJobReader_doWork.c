#include <net/lfcSocketJobReader.h>
#include <asm/errno.h>
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>
#include <core/lfcCore.h>

// --pattern "spec_lfcSocketJobReader*/*"
#define TEST_SUITE_NAME               spec_lfcSocketJobReader_doWork

/**
 * --single=spec_lfcSocketJobReader_doWork::read_complete_in_one_chunk
 */
Test(
    TEST_SUITE_NAME,
    read_complete_in_one_chunk
) {
    int context;
    int ident;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(
        pipe_fds[0],
        &context, &ident,
        0,
        strlen(buf),
        lambda(void, (void *res_context, void *res_ident, ssize_t res_read_len, char *res_buf) {
            should_be_same_ptr_wText(res_context, &context, "callback-context stimmt nicht");
            should_be_same_ptr_wText(res_ident, &ident, "callback-ident stimmt nicht");
            should_be_same_int_wText(res_read_len, strlen(buf), "Anzahl gelesender Bytes stimmt nicht");
            should_be_same_array_wText(res_buf, buf, strlen(buf), "Daten stimmen nicht");
        })
    );

    write(pipe_fds[1], buf, strlen(buf));

    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    delete(tto);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

/**
 * --single=spec_lfcSocketJobReader_doWork::read_complete_in_one_chunk
 */
Test(
    TEST_SUITE_NAME,
    read_complete_in_more_chunk
) {
    int context;
    int ident;
    int pipe_fds[2];

    char *buf = "hellsso";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(
        pipe_fds[0],
        &context, &ident,
        0,
        strlen(buf),
        lambda(void, (void *res_context, void *res_ident, ssize_t res_read_len, char *res_buf) {
            should_be_same_ptr_wText(res_context, &context, "callback-context stimmt nicht");
            should_be_same_ptr_wText(res_ident, &ident, "callback-ident stimmt nicht");
            should_be_same_int_wText(res_read_len, strlen(buf), "Anzahl gelesender Bytes stimmt nicht");
            should_be_same_array_wText(res_buf, buf, strlen(buf), "Daten stimmen nicht");
        })
    );

    write(pipe_fds[1], buf, 1);
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit noch nicht fertig ist");

    write(pipe_fds[1], &buf[1], strlen(buf) - 1);
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    delete(tto);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

/**
 * --single=spec_lfcSocketJobReader_doWork::call_on_a_complete_read
 */
Test(
    TEST_SUITE_NAME,
    call_on_a_complete_read
) {
    int context;
    int ident;
    unsigned char callback_call_counter = 0;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(
        pipe_fds[0],
        &context, &ident,
        0,
        strlen(buf),
        lambda(void, (void *res_context isAnUnused_param, void *res_ident isAnUnused_param, ssize_t res_read_len isAnUnused_param, char *res_buf isAnUnused_param) {
            callback_call_counter++;
        })
    );


    write(pipe_fds[1], buf, strlen(buf));

    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    should_be_same_int_wText(callback_call_counter, 1, "callback mehr als einmal aufgerufen");

    delete(tto);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

/**
 * --single=spec_lfcSocketJobReader_doWork::read_on_closed_fd
 */
Test(
    TEST_SUITE_NAME,
    read_on_closed_fd
) {
    int context;
    int ident;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(
        pipe_fds[0],
        &context, &ident,
        0,
        strlen(buf),
        lambda(void, (void *res_context, void *res_ident, ssize_t res_read_len, char *res_buf isAnUnused_param) {
            should_be_same_ptr_wText(res_context, &context, "callback-context stimmt nicht");
            should_be_same_ptr_wText(res_ident, &ident, "callback-ident stimmt nicht");
            should_be_same_int_wText(res_read_len, -EBADF, "Anzahl gelesender Bytes stimmt nicht");
        })
    );

    close(pipe_fds[0]);
    close(pipe_fds[1]);

    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), -EBADF, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    delete(tto);
}

/**
 * --single=spec_lfcSocketJobReader_doWork::repeat_0
 */
Test(
    TEST_SUITE_NAME,
    repeat_0
) {
    int context;
    int ident;
    unsigned char callback_call_counter = 0;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(
        pipe_fds[0],
        &context, &ident,
        0,
        0,
        calloc(1, strlen(buf)),
        strlen(buf),
        true,
        lambda(void, (void *res_context isAnUnused_param, void *res_ident isAnUnused_param, ssize_t res_read_len isAnUnused_param, char *res_buf isAnUnused_param) {
            callback_call_counter++;
            if (callback_call_counter == 2) {
                free(res_buf);
            }
        })
    );


    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    should_be_same_int_wText(callback_call_counter, 5, "callback mehr als einmal aufgerufen");

    delete(tto);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

/**
 * --single=spec_lfcSocketJobReader_doWork::repeat_1
 */
Test(
    TEST_SUITE_NAME,
    repeat_1
) {
    int context;
    int ident;
    unsigned char callback_call_counter = 0;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(
        pipe_fds[0],
        &context, &ident,
        0,
        1,
        calloc(1, strlen(buf)),
        strlen(buf),
        true,
        lambda(void, (void *res_context isAnUnused_param, void *res_ident isAnUnused_param, ssize_t res_read_len isAnUnused_param, char *res_buf isAnUnused_param) {
            callback_call_counter++;
        })
    );


    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    should_be_same_int_wText(callback_call_counter, 1, "callback mehr als einmal aufgerufen");

    delete(tto);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

/**
 * --single=spec_lfcSocketJobReader_doWork::repeat_2
 */
Test(
    TEST_SUITE_NAME,
    repeat_2
) {
    int context;
    int ident;
    unsigned char callback_call_counter = 0;
    int pipe_fds[2];

    char *buf = "hello";

    pipe(pipe_fds);

    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(
        pipe_fds[0],
        &context, &ident,
        0,
        2,
        calloc(1, strlen(buf)),
        strlen(buf),
        true,
        lambda(void, (void *res_context isAnUnused_param, void *res_ident isAnUnused_param, ssize_t res_read_len isAnUnused_param, char *res_buf isAnUnused_param) {
            callback_call_counter++;

//            if (callback_call_counter == 2) {
//                free(res_buf);
//            }
        })
    );


    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 0, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    write(pipe_fds[1], buf, strlen(buf));
    should_be_same_int_wText(lfcSocketJob_doWork(asInstanceOf(lfcSocketJob(), tto)), 1, "doWork hat nicht zurueckgegeben, dass die Arbeit fertig ist");

    should_be_same_int_wText(callback_call_counter, 2, "callback mehr als einmal aufgerufen");
    should_be_false_wText(lfcSocketJob_isEnabled(asInstanceOf(lfcSocketJob(), tto)), "job sollte jetzt deaktiviert sein");

    delete(tto);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}
