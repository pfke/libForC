#ifndef LIBFORC_DATA_STRINGBUILDER_H_
#define LIBFORC_DATA_STRINGBUILDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/lfcObject.h"
#include "../collection/lfcList.h"
#include <stdlib.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcStringBuilder)

struct lfcStringBuilder { const struct lfcObject _;
    lfcList_t *stringList;
};

struct lfcStringBuilder_class { const struct lfcObject_class _;
    method_t append;
    method_t appendChar;
    method_t append_itsYourMemNow;
    method_t result;
};

struct lfcStringBuilder_methods {
    int (*append)(lfcStringBuilder_t *self, const char *in);
    int (*appendChar)(lfcStringBuilder_t *self, char in);
    int (*append_itsYourMemNow)(lfcStringBuilder_t *self, char *in);

    char * (*result)(lfcStringBuilder_t *self);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcStringBuilderger Instanz.
 */
lfcStringBuilder_t *lfcStringBuilder_ctor (void);

int lfcStringBuilder_append(lfcStringBuilder_t *self, const char *in);
int lfcStringBuilder_appendChar(lfcStringBuilder_t *self, char in);
int lfcStringBuilder_append_itsYourMemNow(lfcStringBuilder_t *self, char *in);
char * lfcStringBuilder_result(lfcStringBuilder_t *self);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_DATA_STRINGBUILDER_H_ ---*/
