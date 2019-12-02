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
lfcOOP_defineClass(lfcStringBuilder, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    lfcList_t *stringList;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    int, append, (const char *in),
    int, append_itsYourMemNow, (char *in),

    char *, result, ()
    )

/**
 * Erzeugt eine lfcStringBuilderger Instanz.
 */
lfcStringBuilder_t *lfcStringBuilder_ctor (void);


#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_DATA_STRINGBUILDER_H_ ---*/
