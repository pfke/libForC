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

#endif //LIBFORC_CORE_LFCCORE_H
