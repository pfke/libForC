#ifndef LIBFORC_TESTING_LFCCRITERIONHELPER_H
#define LIBFORC_TESTING_LFCCRITERIONHELPER_H

    #include <criterion/criterion.h>
    #include <mcheck.h>

    #define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))

    #define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

    char *convert_array_to_hexstr(const char *in, size_t len);
    void printout_array(char *buf, size_t len);

//    #define CHECK_ALLOC_VIA_MPROBE
    #ifdef CHECK_ALLOC_VIA_MPROBE
        const char *lfcCriterionHelper_mprobe_result_to_string(enum mcheck_status in);
    #endif // #ifdef CHECK_ALLOC_VIA_MPROBE

    // Hinweis:
    // Da den Defines auch Funktionen übergeben werden können, müssen die Parameter auf lokale Variablen geschrieben werden,
    // da sonst die 2malige Referenz, Funktionen 2mal ausführen lässt.

    #define STR_FOR_NO_DESCR        "no descr"

    /*----------------------------------------------------------------*/
    /* BOOL                                                           */
    /*----------------------------------------------------------------*/
    #define should_be_false(a) should_be_false_wText(a, STR_FOR_NO_DESCR)
    #define should_be_false_wText(a, text)                                                                                  \
        {                                                                                                                   \
            __typeof__(a) _a = (a);                                                                                         \
            cr_assert(_a == false, "[GIVEN] '%s' = %s, [EXPECTED] '%s' = FALSE => " text, #a, (_a) ? "TRUE" : "FALSE", #a); \
        }

    #define should_be_true(a) should_be_true_wText(a, STR_FOR_NO_DESCR)
    #define should_be_true_wText(a, text)                                                                                   \
        {                                                                                                                   \
            __typeof__(a) _a = (a);                                                                                         \
            cr_assert(_a == true, "[GIVEN] '%s' = %s, [EXPECTED] '%s' = TRUE => " text, #a, (_a) ? "TRUE" : "FALSE", #a);   \
        }

    /*----------------------------------------------------------------*/
    /* INTEGER                                                        */
    /*----------------------------------------------------------------*/
    #define should_be_same_int(a, b) should_be_same_int_wText(a, b, STR_FOR_NO_DESCR)
    #define should_be_same_int_wText(a, b, text) \
        {                                                                                                               \
            __typeof__(a) _a = (a);                                                                                     \
            __typeof__(b) _b = (b);                                                                                     \
            cr_assert(_a == _b, "[GIVEN] '%s' = %d, [EXPECTED] '%s' = %d => " text, #a, _a, #b, _b);                   \
        }
    #define should_not_be_same_int(a, b) should_not_be_same_int_wText(a, b, STR_FOR_NO_DESCR)
    #define should_not_be_same_int_wText(a, b, text) \
        {                                                                                                               \
            __typeof__(a) _a = (a);                                                                                     \
            __typeof__(b) _b = (b);                                                                                     \
            cr_assert(_a != _b, "[GIVEN] '%s' = %d, [NOT EXPECTED] '%s' = %d => " text, #a, _a, #b, _b);               \
        }

    /*----------------------------------------------------------------*/
    /* POINTER                                                        */
    /*----------------------------------------------------------------*/
    #define should_be_null(a) \
        {                                                                                                               \
            __typeof__(a) _a = (a);                                                                                     \
            cr_assert(_a == NULL, "[GIVEN] '%s' = %p is not NULL", #a, _a);                                             \
        }

    #define should_not_be_null(a) \
        {                                                                                                               \
            __typeof__(a) _a = a;                                                                                       \
            cr_assert(_a != NULL, "[GIVEN] '%s' = %p is NULL", #a, _a);                                                 \
        }

#ifdef CHECK_ALLOC_VIA_MPROBE
    #define should_be_freed(a) \
        {                                                                                                               \
            enum mcheck_status _a = mprobe(a);                                                                          \
            cr_assert(_a != MCHECK_OK && _a != MCHECK_DISABLED, "[GIVEN] 'mprobe(%s)' = %u (%s) was not freed", #a, _a, lfcCriterionHelper_mprobe_result_to_string(_a));   \
        }

    #define should_not_be_freed(a) \
        {                                                                                                               \
            enum mcheck_status _a = mprobe(a);                                                                          \
            cr_assert(_a == MCHECK_OK, "[GIVEN] 'mprobe(%s)' = %u (%s) was freed", #a, _a, lfcCriterionHelper_mprobe_result_to_string(_a));   \
        }
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE

    #define should_be_same_array(a, b, size) should_be_same_array_wText(a, b, size, STR_FOR_NO_DESCR)
    #define should_be_same_array_wText(a, b, size, text) \
        {                                                                                                               \
            __typeof__(a) _a = (a);                                                                                     \
            __typeof__(b) _b = (b);                                                                                     \
            char *bufa = convert_array_to_hexstr((const char*)_a, (size_t)size);                                        \
            char *bufb = convert_array_to_hexstr((const char*)_b, (size_t)size);                                        \
            cr_assert(memcmp(a, b, size) == 0, "[GIVEN] '%s' = %s, [EXPECTED] '%s' = %s => " text, #a, bufa, #b, bufb); \
            free(bufa);                                                                                                 \
            free(bufb);                                                                                                 \
        }

    #define should_be_a_cleared_array(a, value, size) should_be_a_cleared_array_wText(a, value, size, STR_FOR_NO_DESCR)
    #define should_be_a_cleared_array_wText(a, value, size, text) \
        {                                                                                                               \
            char b[size];                                                                                               \
            memset(b, value, sizeof(b));                                                                                \
            __typeof__(a) _a = (a);                                                                                     \
            char *bufa = convert_array_to_hexstr((const char*)_a, (size_t)size);                                        \
            char *bufb = convert_array_to_hexstr((const char*)b, (size_t)size);                                         \
            cr_assert(memcmp(a, b, size) == 0, "[GIVEN] '%s' = %s, [EXPECTED] = %s => " text, #a, bufa, bufb);          \
            free(bufa);                                                                                                 \
            free(bufb);                                                                                                 \
        }

    #define should_be_same_ptr(a, b) should_be_same_ptr_wText(a, b, STR_FOR_NO_DESCR)
    #define should_be_same_ptr_wText(a, b, text) \
        {                                                                                                               \
            const void *_a = (const void *)(a);                                                                         \
            const void *_b = (const void *)(b);                                                                         \
            cr_assert(_a == _b, "[GIVEN] '%s' = %p, [EXPECTED] '%s' = %p => " text, #a, _a, #b, _b);                    \
        }

    #define should_not_be_same_ptr(a, b) should_not_be_same_ptr_wText(a, b, STR_FOR_NO_DESCR)
    #define should_not_be_same_ptr_wText(a, b, text) \
        {                                                                                                               \
            const void *_a = (const void *)(a);                                                                         \
            const void *_b = (const void *)(b);                                                                         \
            cr_assert(_a != _b, "[GIVEN] '%s' = %p, [NOT EXPECTED] '%s' = %p =>" text, #a, _a, #b, _b);                 \
        }

    #define should_be_same_str(a, b) should_be_same_str_wText(a, b, STR_FOR_NO_DESCR)
    #define should_be_same_str_wText(a, b, text) \
        {                                                                                                               \
            const char *_a = (const char*)a;                                                                            \
            const char *_b = (const char*)b;                                                                            \
            cr_assert(strlen(_a) == strlen(_b), "[GIVEN] '%s' = '%s' (%zu chars), [EXPECTED] '%s' = '%s' (%zu chars) => %s", #a, _a, strlen(_a), #b, _b, strlen(_b), text);     \
            cr_assert(strcmp(_a, _b) == 0, "[GIVEN] '%s' = '%s', [EXPECTED] '%s' = '%s' => %s", #a, _a, #b, _b, text);     \
        }

    /**
     * Diese Funktion wartet maximal die angegebenen Timeout-Zeit und prüft alle <code>interval_in_usec</code>, ob sich der
     * Wert des ptr <code>must_be_true_ptr</code> auf true geöndert hat.
     *
     * @param timeout_in_usec Gesamttimeout in usec
     * @param interval_in_usec Interval in usec
     * @param must_be_true_ptr diese Variable wird geprüft
     * @return 0 .. Timeout ist abgelaufen und Var ist nicht 1
     */
    int lfcCriterionHelper_waitUntil_varIsTrue (
        __USECONDS_T_TYPE timeout_in_usec,
        __USECONDS_T_TYPE interval_in_usec,
        bool *must_be_true_ptr
    );
//    #define waitUntil_by_sec(timeout, interval, ptr)        lfcCriterionHelper_waitUntil_varIsTrue(timeout, interval, ptr)
//    #define waitUntil_by_msec(timeout, interval, ptr)       waitUntil_by_sec(timeout * 1000, interval * 1000, ptr)
//    #define waitUntil_by_usec(timeout, interval, ptr)       waitUntil_by_msec(timeout * 1000, interval * 1000, ptr)

#endif //LIBFORC_TESTING_LFCCRITERIONHELPER_H
