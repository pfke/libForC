#ifndef LIBFORC_TESTING_LFCBBCUNITHELPER_H
#define LIBFORC_TESTING_LFCBBCUNITHELPER_H

#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#define STRINGIZE(x)  #x

#define LFCBCUNITH_TEST(f) \
    void f(void **state); \
    void f (                                                \
        void **state __attribute__((unused))                \
    )

//    lfcBCUnitHelper_registerTest(f);

int lfcBCUnitHelper_registerTest(
//    const char *name,
    CMUnitTestFunction test_func
//    CMFixtureFunction setup_func,
//    CMFixtureFunction teardown_func,
//    void *initial_state
);


#endif //LIBFORC_TESTING_LFCBBCUNITHELPER_H
