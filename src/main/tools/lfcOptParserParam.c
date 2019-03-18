#include "lfcOptParserCmd.h"

#include <stdlib.h>
#include <asm/errno.h>
#include <stdbool.h>
#include <collection/lfcList.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcOptParserParam_t *_lfcOptParserParam;
static const lfcOptParserParam_class_t *_lfcOptParserParam_class;
static lfcOptParserParam_methods_t _lfcOptParserParam_methods __attribute__((unused));


/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static lfcOptParserParam_t *public_lfcOptParserParam_ctor  (void *_self, va_list *app);
static lfcOptParserParam_t *public_lfcOptParserParam_dtor  (lfcOptParserParam_t *self);
static int                public_lfcOptParserParam_parse   (lfcOptParserParam_t *self, const char *in);

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


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
static lfcOptParserParam_t *public_lfcOptParserParam_ctor(
    void *_self,
    va_list *app
) {
    lfcOptParserParam_t *self = (lfcOptParserParam_t *) lfcObject_super_ctor(lfcOptParserParam(), _self);

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
static lfcOptParserParam_t *public_lfcOptParserParam_dtor(
    lfcOptParserParam_t *self
) {
    lfcObject_super_dtor(lfcOptParserParam(), self);

    if (self->name) { free(self->name); }
    if (self->help) { free(self->help); }

    return self;
}

/**
 * Parsed einen Eingabestring
 */
static int public_lfcOptParserParam_parse(
    lfcOptParserParam_t *self,
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
CLASS_CTOR__START(lfcOptParserParam)
        OVERRIDE_METHOD(lfcOptParserParam, parse)
    CLASS_CTOR__INIT_SUPER(lfcOptParserParam, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcOptParserParam_class_t *lfcOptParserParam_class() {
    return _lfcOptParserParam_class
           ? _lfcOptParserParam_class
           : (_lfcOptParserParam_class = (lfcOptParserParam_class_t *) new(
            lfcObject_class(), "lfcOptParserParam_class", lfcObject_class(), sizeof(lfcOptParserParam_class_t),

            lfcObject_ctor, "", impl_lfcOptParserParam_class_ctor,

            (void *) 0)
           );
}

const lfcOptParserParam_t *lfcOptParserParam() {
    return _lfcOptParserParam
           ? _lfcOptParserParam
           : (_lfcOptParserParam = (lfcOptParserParam_t *) new(
            lfcOptParserParam_class(),
            "lfcOptParserParam", lfcObject(), sizeof(lfcOptParserParam_t),

            lfcObject_ctor, "ctor", public_lfcOptParserParam_ctor,
            lfcObject_dtor, "dtor", public_lfcOptParserParam_dtor,

            lfcOptParserParam_parse, "parse", public_lfcOptParserParam_parse,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcOptParserParam);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 */
lfcOptParserParam_t *lfcOptParserParam_ctor(
    const char *name,
    const char *help
) {
    return (lfcOptParserParam_t *)new(lfcOptParserParam(), name, help);
}


IMPL_API__wRET__w_1PARAM(lfcOptParserParam, parse, int, const char *)
