#include "lfcCriterionHelper.h"
#include <stdio.h>
#include <unistd.h>

char *convert_array_to_hexstr(
    const char *in,
    size_t len
) {
    size_t i;
    char *result = calloc(1, len * 2 + 2 + 1); // +2 = "0x"; +1 = '\0'
    char *out = result + 2;

    if(result == NULL) {
        return NULL;
    }

    result[0] = '0';
    result[1] = 'x';

    for(i=0; i<len; i++) {
        char half;

        half = (in[i] >> 4) & 0xf;
        if (half < 0xa) {
            out[i*2] = half + 0x30;
        } else {
            out[i*2] = half + 0x61 - 0xa;
        }

        half = in[i] & 0xf;
        if (half < 0xa) {
            out[i*2 + 1] = half + 0x30;
        } else {
            out[i*2 + 1] = half + 0x61 - 0xa;
        }

    }

    return result;
}


void printout_array(char *buf, size_t len) {
    size_t i;

    fprintf(stderr, "=====\n");
    for( i = 0; i < len; i++) {
        fprintf(stderr, "%02x ", buf[i] & 0xff);

        if (((i + 1) % 10) == 0)
            fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}

// das folgende rausgenommen, weil es für einen seg-fault in criterion sorgt
#ifdef CHECK_ALLOC_VIA_MPROBE
/**
 * Constructor zeuch beim unit testing.
 *
 * Hier wird mcheck initialisiert. Dieses MUSS vor dem ersten alloc geschehen!!!
 */
static void no_op(enum mcheck_status status) {}
static void lfcCriterionHelper_ctor() __attribute__((constructor (101)));
static void lfcCriterionHelper_ctor() {
    //mcheck_pedantic(no_op);
}

const char *lfcCriterionHelper_mprobe_result_to_string(
    enum mcheck_status in
) {
    switch(in) {
        case MCHECK_DISABLED: return "MCHECK_DISABLED: Consistency checking is not turned on";
        case MCHECK_OK:       return "MCHECK_OK: Block is fine";
        case MCHECK_FREE:     return "MCHECK_FREE: Block freed twice";
        case MCHECK_HEAD:     return "MCHECK_HEAD: Memory before the block was clobbered";
        case MCHECK_TAIL:     return "MCHECK_TAIL: Memory after the block was clobbered";

        default: return "INVALID";
    }
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE

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
) {
//    fprintf(stderr, "%d@%s\n", __LINE__, __func__);
    for (__USECONDS_T_TYPE i = 0; i < timeout_in_usec; i += interval_in_usec) {
        if (*must_be_true_ptr == true) {
            return 1;
        }
        usleep(interval_in_usec);
    }

    return 0;
}