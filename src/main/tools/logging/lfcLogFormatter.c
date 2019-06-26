#include "lfcLogFormatter.h"

#include "../../data/lfcStringBuilder.h"
#include "lfcLog.h"
#include "lfcLogCommon.h"
#include <stdbool.h>
#include <string.h>


char *lfcLogFormatter_formatAsString(
    const char *fmt,
    const lfcLog_t *log
) {
    if (!fmt) { return strdup("!!! no fmt string passed !!!"); }
    if (!log) { return strdup("!!! no log item !!!"); }

    // fmt parsen
    lfcStringBuilder_t *stringBuilder = lfcStringBuilder_ctor();
    if (!stringBuilder) { return strdup("!!! no mem !!!"); }

    size_t fmtLen = strlen(fmt);
    size_t i, lastcopiedPos = 0;
    bool searchForPattern = true;

    for (i = 0; i < fmtLen; i++) {
        if (searchForPattern) {
            if (fmt[i] != '%') {
                continue;
            } else {
                lfcStringBuilder_append_itsYourMemNow(stringBuilder, strndup(&fmt[lastcopiedPos], i - lastcopiedPos));

                searchForPattern = false;
                continue;
            }
        }

        switch (fmt[i]) {
            case '%':
                lfcStringBuilder_append(stringBuilder, "%");
                break;

            case 'f': {
                char buf[20];

                snprintf(buf, sizeof(buf), "%d", lfcLog_getMethodLine((lfcLog_t *)log));

                lfcStringBuilder_append(stringBuilder, buf);

                break;
            }
            case 'F':
                lfcStringBuilder_append(stringBuilder, lfcLog_getMethod((lfcLog_t *)log));
                break;

            case 'l':
                lfcStringBuilder_append(stringBuilder, lfcLogCommon_logLevel_to_shortStr(lfcLog_getLogLevel((lfcLog_t *)log)));
                break;
            case 'L':
                lfcStringBuilder_append(stringBuilder, lfcLogCommon_logLevel_to_longStr(lfcLog_getLogLevel((lfcLog_t *)log)));
                break;
            case 'c':
                lfcStringBuilder_append(stringBuilder, lfcLogCommon_logLevel_to_ansiEscCode(lfcLog_getLogLevel((lfcLog_t *)log)));
                break;
            case 'C':
                lfcStringBuilder_append(stringBuilder, lfcLogCommon_logLevel_reset_ansiEscCode(lfcLog_getLogLevel((lfcLog_t *)log)));
                break;

            case 'm':
                lfcStringBuilder_append(stringBuilder, lfcLog_getMessage((lfcLog_t *)log));
                break;

            case 'p':
                lfcStringBuilder_append(stringBuilder, lfcLog_getLoggerPrefix((lfcLog_t *)log));
                break;

            case 't': {
                char buf[100];

                memset(buf, 0, sizeof(buf));
                snprintf(buf, sizeof(buf), "%lld", (long long int)lfcLog_getThreadId((lfcLog_t *)log));

                lfcStringBuilder_append(stringBuilder, buf);

                break;
            }
            case 'T':
                lfcStringBuilder_append(stringBuilder, lfcLog_getThreadName((lfcLog_t *)log));
                break;

            case 'D': {
                char buf[100];
                char fmtBuf[100];

                // get the fmt string
                char *end = strchr(&fmt[i], '}');
                memset(fmtBuf, 0, sizeof(fmtBuf));
                strncat(fmtBuf, &fmt[i + 2], end - &fmt[i + 2]);

                snprintf(buf, sizeof(buf), "%ld", lfcLog_getThreadId((lfcLog_t *)log));

                time_t timeStamp = lfcLog_getTimeStamp((lfcLog_t *)log);
                struct tm *local_time = localtime(&timeStamp);
                if(!local_time) {
                    lfcStringBuilder_append(stringBuilder, "!!! time fmt error !!!");
                } else {
                    strftime(buf, sizeof(buf)-1, fmtBuf, local_time);
                    lfcStringBuilder_append(stringBuilder, buf);
                }
                i = end - fmt;

                break;
            }

            default:
                break;
        }

        lastcopiedPos = i + 1;
        searchForPattern = true;
    }
    lfcStringBuilder_append_itsYourMemNow(stringBuilder, strndup(&fmt[lastcopiedPos], i - lastcopiedPos));

    char *result = lfcStringBuilder_result(stringBuilder);
    delete(stringBuilder);

    return result;
}
