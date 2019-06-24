#include "lfcOptParser.h"

#include <stdlib.h>
#include <asm/errno.h>
#include <stdbool.h>
#include "../collection/lfcList.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcOptParser_t *_lfcOptParser;
static const lfcOptParser_class_t *_lfcOptParser_class;
static lfcOptParser_methods_t _lfcOptParser_methods __attribute__((unused));


/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static bool private_lfcOptParser_ensureList(lfcList_t **pList);

static lfcOptParser_t *public_lfcOptParser_ctor  (void *_self, va_list *app);
static lfcOptParser_t *public_lfcOptParser_dtor  (lfcOptParser_t *self);
static int             public_lfcOptParser_addCmd  (lfcOptParser_t *self, lfcOptParserCmd_t *toAdd);
static int             public_lfcOptParser_addParam(lfcOptParser_t *self, lfcOptParserParam_t *toAdd);
static int             public_lfcOptParser_dump  (lfcOptParser_t *self, FILE *fp);
static int             public_lfcOptParser_parse (lfcOptParser_t *self, const char *in);

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

/**
 * Stellt sicher, dass die übergebene Liste instanziiert wird, fall sie noch nicht existiert.
 */
static bool private_lfcOptParser_ensureList (
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
static lfcOptParser_t *public_lfcOptParser_ctor(
    void *_self,
    va_list *app __attribute__((unused))
) {
    lfcOptParser_t *self = (lfcOptParser_t *) lfcObject_super_ctor(lfcOptParser(), _self);

    if (self == 0)
        return 0;

    // read args

    return self;
}

/**
 * Destructor selbst.
 */
static lfcOptParser_t *public_lfcOptParser_dtor(
    lfcOptParser_t *self
) {
    lfcObject_super_dtor(lfcOptParser(), self);

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

    return self;
}

/**
 * Mit dieser Methode werden Optionen hinzugefuegt
 */
static int public_lfcOptParser_addCmd(
    lfcOptParser_t *self,
    lfcOptParserCmd_t *toAdd
) {
    if (!toAdd) { return -EINVAL; }
    if (!private_lfcOptParser_ensureList(&self->cmdList)) { return -ENOMEM; }

    return lfcList_add(self->cmdList, toAdd);
}

/**
 * Mit dieser Methode werden Params hinzugefuegt
 */
static int public_lfcOptParser_addParam(
    lfcOptParser_t *self,
    lfcOptParserParam_t *toAdd
) {
    if (!toAdd) { return -EINVAL; }
    if (!private_lfcOptParser_ensureList(&self->paramList)) { return -ENOMEM; }

    return lfcList_add(self->paramList, toAdd);
}

/**
 * Diese Methode printet die komplette Konfiguration
 */
static int public_lfcOptParser_dump(
    lfcOptParser_t *self,
    FILE *fp
) {
    if (!fp) { return -EINVAL; }

    return 0;
}

/**
 * Parsed einen Eingabestring
 */
static int public_lfcOptParser_parse(
    lfcOptParser_t *self,
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
CLASS_CTOR__START(lfcOptParser)
        OVERRIDE_METHOD(lfcOptParser, addCmd)
        OVERRIDE_METHOD(lfcOptParser, addParam)
        OVERRIDE_METHOD(lfcOptParser, dump)
        OVERRIDE_METHOD(lfcOptParser, parse)
    CLASS_CTOR__INIT_SUPER(lfcOptParser, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()
const lfcOptParser_class_t *lfcOptParser_class() {
    return _lfcOptParser_class
           ? _lfcOptParser_class
           : (_lfcOptParser_class = (lfcOptParser_class_t *) new(
            lfcObject_class(), "lfcOptParser_class", lfcObject_class(), sizeof(lfcOptParser_class_t),

            lfcObject_ctor, "", impl_lfcOptParser_class_ctor,

            (void *) 0)
           );
}

const lfcOptParser_t *lfcOptParser() {
    return _lfcOptParser
           ? _lfcOptParser
           : (_lfcOptParser = (lfcOptParser_t *) new(
            lfcOptParser_class(),
            "lfcOptParser", lfcObject(), sizeof(lfcOptParser_t),

            lfcObject_ctor, "ctor", public_lfcOptParser_ctor,
            lfcObject_dtor, "dtor", public_lfcOptParser_dtor,

            lfcOptParser_addCmd, "addCmd", public_lfcOptParser_addCmd,
            lfcOptParser_addParam, "addParam", public_lfcOptParser_addParam,
            lfcOptParser_dump, "dump", public_lfcOptParser_dump,
            lfcOptParser_parse, "parse", public_lfcOptParser_parse,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcOptParser);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Liste mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcOptParser_t *lfcOptParser_ctor(
) {
    return (lfcOptParser_t *)new(lfcOptParser());
}

lfcOptParserCmd_t *lfcOptParser_hlpr_addCmd (
    lfcOptParser_t *self,
    const char *name,
    const char *help
) {
    if (!self) { return NULL; }

    lfcOptParserCmd_t *toAdd = lfcOptParserCmd_ctor(name, help);
    if (0 == lfcOptParser_addCmd(self, toAdd)) {
        delete(toAdd);
        toAdd = NULL;
    }

    return toAdd;
}

lfcOptParserParam_t *lfcOptParser_hlpr_addParam (
    lfcOptParser_t *self,
    const char *name,
    const char *help
) {
    if (!self) { return NULL; }

    lfcOptParserParam_t *toAdd = lfcOptParserParam_ctor(name, help);
    if (0 == lfcOptParser_addParam(self, toAdd)) {
        delete(toAdd);
        toAdd = NULL;
    }

    return toAdd;
}

lfcOOP_accessor(lfcOptParser, addCmd, int, lfcOptParserCmd_t *)
lfcOOP_accessor(lfcOptParser, addParam, int, lfcOptParserParam_t *)
lfcOOP_accessor(lfcOptParser, dump, int, FILE *)
lfcOOP_accessor(lfcOptParser, parse, int, const char *)
