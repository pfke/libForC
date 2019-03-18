#include "lfcOptParserCmd.h"

#include <stdlib.h>
#include <asm/errno.h>
#include <stdbool.h>
#include <collection/lfcList.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcOptParserCmd_t *_lfcOptParserCmd;
static const lfcOptParserCmd_class_t *_lfcOptParserCmd_class;
static lfcOptParserCmd_methods_t _lfcOptParserCmd_methods __attribute__((unused));


/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static bool private_lfcOptParserCmd_ensureList(lfcList_t **pList);

static lfcOptParserCmd_t *public_lfcOptParserCmd_ctor  (void *_self, va_list *app);
static lfcOptParserCmd_t *public_lfcOptParserCmd_dtor  (lfcOptParserCmd_t *self);
static int                public_lfcOptParserCmd_addCmd  (lfcOptParserCmd_t *self, lfcOptParserCmd_t *toAdd);
static int                public_lfcOptParserCmd_addParam(lfcOptParserCmd_t *self, lfcOptParserParam_t *toAdd);
static int                public_lfcOptParserCmd_parse   (lfcOptParserCmd_t *self, const char *in);

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

/**
 * Stellt sicher, dass die übergebene Liste instanziiert wird, fall sie noch nicht existiert.
 */
static bool private_lfcOptParserCmd_ensureList (
    lfcList_t **pList
) {
    if (*pList == NULL) {
        *pList = lfcList_ctor();
    }

    return 0;
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                        */
/******************************************************************************************/

/**
* Constructor der Liste selbst.
*
* @param _self die bereits angelegte Instanz der Liste selbst
* @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
*
* @return NULL .. wenn kein Speicher für das interne Array da ist
*         sonst != NULL
*/
static lfcOptParserCmd_t *public_lfcOptParserCmd_ctor(
    void *_self,
    va_list *app
) {
    lfcOptParserCmd_t *self = (lfcOptParserCmd_t *) lfcObject_super_ctor(lfcOptParserCmd(), _self);

    if (self == 0)
        return 0;

    // read args
    self->name = strdup(va_arg(*app, const char *));
    self->help = strdup(va_arg(*app, const char *));
    if (!self->name || !self->help) { goto err_args; }

    return self;

err_args:
    if (self->name) { free(self->name); }
    if (self->help) { free(self->help); }
    free(self);

    return NULL;
}

/**
 * Destructor selbst.
 */
static lfcOptParserCmd_t *public_lfcOptParserCmd_dtor(
    lfcOptParserCmd_t *self
) {
    lfcObject_super_dtor(lfcOptParserCmd(), self);

    if (self->cmdList) {
        lfcIIterable_foreach(
            self->cmdList,
            lambda_void((void *item) {
                            delete(item);
                        }));
        delete(self->cmdList);
    }

    if (self->paramList) {
        lfcIIterable_foreach(
            self->paramList,
            lambda_void((void *item) {
                            delete(item);
                        }));
        delete(self->paramList);
    }

    if (self->name) { free(self->name); }
    if (self->help) { free(self->help); }

    return self;
}

/**
 * Mit dieser Methode werden Optionen hinzugefuegt
 */
static int public_lfcOptParserCmd_addCmd(
    lfcOptParserCmd_t *self,
    lfcOptParserCmd_t *toAdd
) {
    if (!toAdd) { return -EINVAL; }
    if (!private_lfcOptParserCmd_ensureList(&self->cmdList)) { return -ENOMEM; }

    return lfcList_add(self->cmdList, toAdd);
}

/**
 * Mit dieser Methode werden Params hinzugefuegt
 */
static int public_lfcOptParserCmd_addParam(
    lfcOptParserCmd_t *self,
    lfcOptParserParam_t *toAdd
) {
    if (!toAdd) { return -EINVAL; }
    if (!private_lfcOptParserCmd_ensureList(&self->paramList)) { return -ENOMEM; }

    return lfcList_add(self->paramList, toAdd);
}

/**
 * Parsed einen Eingabestring
 */
static int public_lfcOptParserCmd_parse(
    lfcOptParserCmd_t *self,
    const char *in
) {
    if (!in) { return -EINVAL; }

    return 0;
}


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcOptParserCmd)
        OVERRIDE_METHOD(lfcOptParserCmd, addCmd)
        OVERRIDE_METHOD(lfcOptParserCmd, addParam)
        OVERRIDE_METHOD(lfcOptParserCmd, parse)
    CLASS_CTOR__INIT_SUPER(lfcOptParserCmd, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcOptParserCmd_class_t *lfcOptParserCmd_class() {
    return _lfcOptParserCmd_class
           ? _lfcOptParserCmd_class
           : (_lfcOptParserCmd_class = (lfcOptParserCmd_class_t *) new(
            lfcObject_class(), "lfcOptParserCmd_class", lfcObject_class(), sizeof(lfcOptParserCmd_class_t),

            lfcObject_ctor, "", impl_lfcOptParserCmd_class_ctor,

            (void *) 0)
           );
}

const lfcOptParserCmd_t *lfcOptParserCmd() {
    return _lfcOptParserCmd
           ? _lfcOptParserCmd
           : (_lfcOptParserCmd = (lfcOptParserCmd_t *) new(
            lfcOptParserCmd_class(),
            "lfcOptParserCmd", lfcObject(), sizeof(lfcOptParserCmd_t),

            lfcObject_ctor, "ctor", public_lfcOptParserCmd_ctor,
            lfcObject_dtor, "dtor", public_lfcOptParserCmd_dtor,

            lfcOptParserCmd_addCmd, "addCmd", public_lfcOptParserCmd_addCmd,
            lfcOptParserCmd_addParam, "addParam", public_lfcOptParserCmd_addParam,
            lfcOptParserCmd_parse, "parse", public_lfcOptParserCmd_parse,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcOptParserCmd);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 */
lfcOptParserCmd_t *lfcOptParserCmd_ctor(
    const char *name,
    const char *help
) {
    return (lfcOptParserCmd_t *)new(lfcOptParserCmd(), name, help);
}

lfcOptParserCmd_t *lfcOptParserCmd_hlpr_addCmd (
    lfcOptParserCmd_t *self,
    const char *name,
    const char *help
) {
    if (!self) { return NULL; }
    if (!name) { return NULL; }
    if (!help) { return NULL; }

    lfcOptParserCmd_t *toAdd = lfcOptParserCmd_ctor(name, help);
    if (0 == lfcOptParserCmd_addCmd(self, toAdd)) {
        delete(toAdd);
        toAdd = NULL;
    }

    return toAdd;
}

lfcOptParserParam_t *lfcOptParserCmd_hlpr_addParam (
    lfcOptParserCmd_t *self,
    const char *name,
    const char *help
) {
    if (!self) { return NULL; }
    if (!name) { return NULL; }
    if (!help) { return NULL; }

    lfcOptParserParam_t *toAdd = lfcOptParserParam_ctor(name, help);
    if (0 == lfcOptParserCmd_addParam(self, toAdd)) {
        delete(toAdd);
        toAdd = NULL;
    }

    return toAdd;
}

IMPL_API__wRET__w_1PARAM(lfcOptParserCmd, addCmd, int, lfcOptParserCmd_t *)
IMPL_API__wRET__w_1PARAM(lfcOptParserCmd, addParam, int, lfcOptParserParam_t *)
IMPL_API__wRET__w_1PARAM(lfcOptParserCmd, parse, int, const char *)
