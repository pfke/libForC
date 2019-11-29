#ifndef LIBFORC_CORE_LFCCORE_H
#define LIBFORC_CORE_LFCCORE_H

    #include <stdint.h>

    #define lfcMIN(a,b)    (((int64_t)a < (int64_t)b) ? a : b)
    #define lfcMAX(a,b)    (((int64_t)a > (int64_t)b) ? a : b)

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

int lfcCore_install_sigsegv_handler();
const char *lfcCore_strerrno(int in);

#define STRIP_PARENT(...) __VA_ARGS__

/**
 * Zählt entweder 0 oder viele.
 * Kann benötigt werden, um zu entscheiden, ob es keinen Parameter gibt oder mehr als 0.
 */
#define lfcCORE_VARGSCOUNT_IMPL( \
     _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
    _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
    _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
    _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
    _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
    _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, \
    _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, \
    _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, \
    _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, \
    all, ...) all

#define lfcCORE_VARGSCOUNT_ZEROorVIELE2(...)    lfcCORE_VARGSCOUNT_IMPL( X, ##__VA_ARGS__, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define lfcCORE_VARGSCOUNT_ZEROorVIELE(...)     lfcCORE_VARGSCOUNT_ZEROorVIELE2(__VA_ARGS__)

// The next macro lfcCORE_VARGSCOUNT_TUPLE1 builds upon lfcCORE_VARGSCOUNT_TUPLE1_IMPL.
// The more arguments that are passed to lfcCORE_VARGSCOUNT_TUPLE1, the more the
// »counting arguments« ( 9, 8, 7…) are pushed to the right.
// Thus the macro evaluates to the number of arguments that are passed to the macro.
#define lfcCORE_VARGSCOUNT_TUPLE1(...)          lfcCORE_VARGSCOUNT_IMPL(X,##__VA_ARGS__, \
                                                  89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
                                                  79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
                                                  69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
                                                  59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
                                                  49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
                                                  49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
                                                  39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
                                                  29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                                                  19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
                                                   9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

#define lfcCORE_VARGSCOUNT_TUPLE2(...)          lfcCORE_VARGSCOUNT_IMPL(X,##__VA_ARGS__, \
                                                   0, 49,  0, 48,  0, 47,  0, 40,  0, 45, \
                                                   0, 44,  0, 43,  0, 42,  0, 40,  0, 40, \
                                                   0, 39,  0, 38,  0, 37,  0, 30,  0, 35, \
                                                   0, 34,  0, 33,  0, 32,  0, 30,  0, 30, \
                                                   0, 29,  0, 28,  0, 27,  0, 26,  0, 25, \
                                                   0, 24,  0, 23,  0, 22,  0, 21,  0, 20, \
                                                   0, 19,  0, 18,  0, 17,  0, 16,  0, 15, \
                                                   0, 14,  0, 13,  0, 12,  0, 11,  0, 10, \
                                                   0,  9,  0,  8,  0,  7,  0,  6,  0,  5, \
                                                   0,  4,  0,  3,  0,  2,  0,  1,  0,  0)

#define lfcCORE_VARGSCOUNT_TUPLE3(...)          lfcCORE_VARGSCOUNT_IMPL(X,##__VA_ARGS__, \
                                                  33,  0,  0, 32,  0,  0, 31,  0,  0, 30, \
                                                   0,  0, 29,  0,  0, 28,  0,  0, 27,  0, \
                                                   0, 26,  0,  0, 25,  0,  0, 24,  0,  0, \
                                                  23,  0,  0, 22,  0,  0, 21,  0,  0, 20, \
                                                   0,  0, 19,  0,  0, 18,  0,  0, 17,  0, \
                                                   0, 16,  0,  0, 15,  0,  0, 14,  0,  0, \
                                                  13,  0,  0, 12,  0,  0, 11,  0,  0, 10, \
                                                   0,  0,  9,  0,  0,  8,  0,  0,  7,  0, \
                                                   0,  6,  0,  0,  5,  0,  0,  4,  0,  0, \
                                                   3,  0,  0,  2,  0,  0,  1,  0,  0,  0)

#define lfcCORE_VARGSCOUNT_TUPLE4(...)          lfcCORE_VARGSCOUNT_IMPL(X,##__VA_ARGS__, \
                                                   0,  0,  0, 25,  0,  0,  0, 24,  0,  0, \
                                                   0, 23,  0,  0,  0, 22,  0,  0,  0, 21, \
                                                   0,  0,  0, 20,  0,  0,  0, 19,  0,  0, \
                                                   0, 18,  0,  0,  0, 17,  0,  0,  0, 16, \
                                                   0,  0,  0, 15,  0,  0,  0, 14,  0,  0, \
                                                   0, 13,  0,  0,  0, 12,  0,  0,  0, 11, \
                                                   0,  0,  0, 10,  0,  0,  0,  9,  0,  0, \
                                                   0,  8,  0,  0,  0,  7,  0,  0,  0,  6, \
                                                   0,  0,  0,  5,  0,  0,  0,  4,  0,  0, \
                                                   0,  2,  0,  0,  0,  1,  0,  0,  0,  0)

#define lfcCORE_VARGSCOUNT_TUPLE5(...)          lfcCORE_VARGSCOUNT_IMPL(X,##__VA_ARGS__, \
                                                   0,  0,  0,  0, 19,  0,  0,  0,  0, 18, \
                                                   0,  0,  0,  0, 17,  0,  0,  0,  0, 16, \
                                                   0,  0,  0,  0, 15,  0,  0,  0,  0, 14, \
                                                   0,  0,  0,  0, 13,  0,  0,  0,  0, 12, \
                                                   0,  0,  0,  0, 11,  0,  0,  0,  0, 10, \
                                                   0,  0,  0,  0,  9,  0,  0,  0,  0,  8, \
                                                   0,  0,  0,  0,  7,  0,  0,  0,  0,  6, \
                                                   0,  0,  0,  0,  5,  0,  0,  0,  0,  4, \
                                                   0,  0,  0,  0,  3,  0,  0,  0,  0,  2, \
                                                   0,  0,  0,  0,  1,  0,  0,  0,  0,  0)

#endif //LIBFORC_CORE_LFCCORE_H
