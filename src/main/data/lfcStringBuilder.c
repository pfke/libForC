#include "lfcStringBuilder.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcStringBuilder, lfcObject,
    int,    append, (const char *),
    int,    append_itsYourMemNow, (char *),
    char *, result, ()
)


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcStringBuilder_t *public_lfcStringBuilder_ctor (
    void *_self,
    va_list *app isAnUnused_param
) {
    lfcStringBuilder_t *self = (lfcStringBuilder_t *) lfcObject_super_ctor(lfcStringBuilder(), _self);
    ASSERT_PTR(self, err, "could not create instance");

    self->stringList = lfcList_ctor();
    if (!self->stringList) { goto err; }

    // read args
    return self;

err:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcStringBuilder_t *public_lfcStringBuilder_dtor (
    lfcStringBuilder_t *self
) {
    lfcIIterable_foreach(self->stringList, lambda_void((void *item) {
        free(item);
    }));
    delete(self->stringList);

    return lfcObject_super_dtor(lfcStringBuilder(), self);
}

static int public_lfcStringBuilder_append(
    lfcStringBuilder_t *self,
    const char *in
) {
    if (!in) { return -EINVAL; }

    lfcList_add(self->stringList, strdup(in));

    return 0;
}

static int public_lfcStringBuilder_append_itsYourMemNow (
    lfcStringBuilder_t *self,
    char *in
) {
    if (!in) { return -EINVAL; }

    lfcList_add(self->stringList, in);

    return 0;
}

static char *public_lfcStringBuilder_result(
    lfcStringBuilder_t *self
) {
    size_t resultLen = 0;

    lfcIIterable_foreach(self->stringList, lambda_void((void *item) {
        resultLen += strlen(item);
    }));

    char *result = calloc(1, resultLen + 1);
    lfcIIterable_foreach(self->stringList, lambda_void((void *item) {
        strcat(result, item);
    }));

    return result;
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Erzeugt eine lfcStringBuilder Instanz.
 */
lfcStringBuilder_t *lfcStringBuilder_ctor () {
    return (lfcStringBuilder_t *)new(lfcStringBuilder());
}

