#include <net/lfcSocketJobReader.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJob_ctor

Test(
    TEST_SUITE_NAME,
    should_set__fd
) {
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, NULL, NULL, 2, 123);

    should_be_same_int(tto->fd, 1345);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__context
) {
    int context;
    int ident;
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, &context, &ident, 2, 123);

    should_be_same_ptr(tto->context, &context);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__ident
) {
    int context;
    int ident;
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, &context, &ident, 2, 123);

    should_be_same_ptr(tto->ident, &ident);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__timeout_in_s
) {
    int context;
    int ident;
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, &context, &ident, 456461, 123);

    should_be_same_int(tto->timeout_in_s, 456461);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__repeat
) {
    int context;
    int ident;
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, &context, &ident, 456461, 123);

    should_be_same_int(tto->repeat, 123);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__enabled
) {
    int context;
    int ident;
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, &context, &ident, 456461, 123);

    should_be_true(tto->enabled);

    delete(tto);
}
