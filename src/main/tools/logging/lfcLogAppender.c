#include "lfcLogAppender.h"
#include "../../core/lfcIface.h"


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcLogAppender, lfcObject,
    int, put, (lfcLog_t *)
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
static lfcLogAppender_t *public_lfcLogAppender_ctor (
    void *_self,
    va_list *app isAnUnused_param
) {
    lfcLogAppender_t *self = (lfcLogAppender_t *) lfcObject_super_ctor(lfcLogAppender(), _self);
    ASSERT_PTR(self, err, "could not create instance");

    // read args
    return self;

err:
    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLogAppender_t *public_lfcLogAppender_dtor (
    lfcLogAppender_t *self
) {
    return lfcObject_super_dtor(lfcLogAppender(), self);
}

static int public_lfcLogAppender_put(
    lfcLogAppender_t *self isAnUnused_param,
    lfcLog_t *log isAnUnused_param
) {
    return 00;
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/
