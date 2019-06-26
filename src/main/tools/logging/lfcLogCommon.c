#include "lfcLogCommon.h"

#include <stdio.h>

const char *lfcLogCommon_logLevel_to_longStr (
    lfcLogging_loglevel_e log_level
) {
    switch (log_level) {
        case LOGLEVEL_EMERG:    return ("EMERG");
        case LOGLEVEL_ALERT:    return ("ALERT");
        case LOGLEVEL_CRIT:     return ("CRIT");
        case LOGLEVEL_ERR:      return ("ERR");
        case LOGLEVEL_WARNING:  return ("WARN");
        case LOGLEVEL_NOTICE:   return ("NOTE");
        case LOGLEVEL_INFO:     return ("INFO");
        case LOGLEVEL_DEBUG:    return ("DEBUG");

        default: return ("??");
    }
}

const char *lfcLogCommon_logLevel_to_shortStr (
    lfcLogging_loglevel_e log_level
) {
    switch (log_level) {
        case LOGLEVEL_EMERG:    return ("UR");
        case LOGLEVEL_ALERT:    return ("AL");
        case LOGLEVEL_CRIT:     return ("CR");
        case LOGLEVEL_ERR:      return ("EE");
        case LOGLEVEL_WARNING:  return ("WW");
        case LOGLEVEL_NOTICE:   return ("NN");
        case LOGLEVEL_INFO:     return ("II");
        case LOGLEVEL_DEBUG:    return ("DD");

        default: return ("??");
    }
}

const char *lfcLogCommon_logLevel_to_ansiEscCode (
    lfcLogging_loglevel_e log_level
) {
    switch (log_level) {
        case LOGLEVEL_EMERG:    return ("\33[30m\33[101m"); // Black on Bright Red
        case LOGLEVEL_ALERT:    return ("\33[30m\33[101m"); // Black on Bright Red
        case LOGLEVEL_CRIT:     return ("\33[91m"); // Bright Red
        case LOGLEVEL_ERR:      return ("\33[91m"); // Bright Red
        case LOGLEVEL_WARNING:  return ("\33[93m"); // Yellow
        case LOGLEVEL_NOTICE:   return ("\33[34m"); // Blue
        case LOGLEVEL_INFO:     return ("\33[94m"); // Bright Blue
        case LOGLEVEL_DEBUG:    return lfcLogCommon_logLevel_reset_ansiEscCode (); // ...

        default: return lfcLogCommon_logLevel_reset_ansiEscCode ();
    }
}

const char *lfcLogCommon_logLevel_reset_ansiEscCode () {
    return ("\33[0m"); // Reset:
}
