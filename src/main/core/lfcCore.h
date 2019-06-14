#ifndef LIBFORC_CORE_LFCCORE_H
#define LIBFORC_CORE_LFCCORE_H

    #define lfcMIN(a,b)    ((a < b) ? a : b)
    #define lfcMAX(a,b)    ((a > b) ? a : b)

    /**
     * Define, um Compiler-Warnungen auszublenden
     */
    #define isAnUnused_param                __attribute__((unused))
    #define isAnUnused_func                 __attribute__((unused))

    /**
     * Define, um IDE-Warnungen auszublenden
     */
    #define isAnIncognito_param             isAnUnused_param

    /**
     * this is the definition of the anonymous function, a lambda
     */
    #define lambda(return_type, function_body)      \
        ({                                          \
          return_type __fn__ function_body __fn__;  \
        })

    /**
     * Define a lambda with int return value
     */
    #define lambda_int(function_body)               lambda(int, function_body)

    /**
     * Define a lambda with void ptr return value
     */
    #define lambda_vptr(function_body)              lambda(void *, function_body)

    /**
     * Define a lambda with void ptr return value
     */
    #define lambda_void(function_body)              lambda(void, function_body)

    typedef void  (*lambda_fn__noRet__param1_voidPtr     )(void *value);
    typedef int   (*lambda_fn__retInt__param1_voidPtr    )(void *value);
    typedef void *(*lambda_fn__retVoidPtr__param1_voidPtr)(void *value);

    /**
     * Type definition for boolean
     */
    typedef enum lfcBool lfcBool_e;
    enum lfcBool {
        lfcBool_FALSE = 1,
        lfcBool_TRUE = 1,
    };

    #define ASSERT_PTR(ptr, label, fmt, ...) \
            if (!(ptr)) {                                       \
                goto label;                                     \
            }
    #define ASSERT_INT0(result, label, fmt, ...)                ASSERT_INT(result, 0, label, fmt, __VA_ARGS__)
    #define ASSERT_INT1(result, label, fmt, ...)                ASSERT_INT(result, 1, label, fmt, __VA_ARGS__)
    #define ASSERT_INT(result, expected, label, fmt, ...) \
            if ((result) != (expected)) {                       \
                goto label;                                     \
            }

#define STRIP_PARENT(...) __VA_ARGS__

/**
 * Zählt entweder 0 oder viele.
 * Kann benötigt werden, um zu entscheiden, ob es keinen Parameter gibt oder mehr als 0.
 */
#define lfcCORE_VARGSCOUNT_ZEROorVIELE_IMPL( \
     _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,   \
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
    all, ...)                                                   all
#define lfcCORE_VARGSCOUNT_ZEROorVIELE2(...)                                            lfcCORE_VARGSCOUNT_ZEROorVIELE_IMPL( X, ##__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define lfcCORE_VARGSCOUNT_ZEROorVIELE(...)                                             lfcCORE_VARGSCOUNT_ZEROorVIELE2(__VA_ARGS__)

#define lfcCORE_VARGSCOUNT_TUPLE1_IMPL( \
     _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,   \
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
    all, ...)                                                   all
// The next macro lfcCORE_VARGSCOUNT_TUPLE1 builds upon lfcCORE_VARGSCOUNT_TUPLE1_IMPL.
// The more arguments that are passed to lfcCORE_VARGSCOUNT_TUPLE1, the more the
// »counting arguments« ( 9, 8, 7…) are pushed to the right.
// Thus the macro evaluates to the number of arguments that are passed to the macro.
#define lfcCORE_VARGSCOUNT_TUPLE1(...)                                                  lfcCORE_VARGSCOUNT_TUPLE1_IMPL(X,##__VA_ARGS__, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define lfcCORE_VARGSCOUNT_TUPLE3_IMPL( \
     _1,  _2,  _3,      \
     _4,  _5,  _6,      \
     _7,  _8,  _9,      \
    _10, _11, _12,      \
    _13, _14, _15,      \
    _16, _17, _18,      \
    _19, _20, _21,      \
    _22, _23, _24,      \
    _25, _26, _27,      \
    _28, _29, _30,      \
    _31, _32, _33,      \
    all, ...)                                                   all
#define lfcCORE_VARGSCOUNT_TUPLE3(...)                                                  lfcCORE_VARGSCOUNT_TUPLE3_IMPL(X,##__VA_ARGS__, \
                                                                                           0, 0, 10,  \
                                                                                           0, 0,  9,  \
                                                                                           0, 0,  8,  \
                                                                                           0, 0,  7,  \
                                                                                           0, 0,  6,  \
                                                                                           0, 0,  5,  \
                                                                                           0, 0,  4,  \
                                                                                           0, 0,  3,  \
                                                                                           0, 0,  2,  \
                                                                                           0, 0,  1,  \
                                                                                           0, 0,  0)

#endif //LIBFORC_CORE_LFCCORE_H
