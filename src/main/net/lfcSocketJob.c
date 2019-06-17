#include "lfcSocketJob.h"

#include "../core/lfcCore.h"

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcSocketJob_t *_lfcSocketJob;
static const lfcSocketJob_class_t *_lfcSocketJob_class;
static lfcSocketJob_methods_t _lfcSocketJob_methods isAnUnused_param;

/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/
static lfcSocketJob_t *public_lfcSocketJob_ctor (void *_self, va_list *app);
static lfcSocketJob_t *public_lfcSocketJob_dtor (lfcSocketJob_t *self);

static bool public_lfcSocketJob_disable (lfcSocketJob_t *self);
static bool public_lfcSocketJob_enable (lfcSocketJob_t *self);
static bool public_lfcSocketJob_isEnabled (lfcSocketJob_t *self);

static int public_lfcSocketJob_getFD(lfcSocketJob_t *self);

/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
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
static lfcSocketJob_t *public_lfcSocketJob_ctor (
    void *_self,
    va_list *app
) {
    lfcSocketJob_t *self = (lfcSocketJob_t *) lfcObject_super_ctor(lfcSocketJob(), _self);

    ASSERT_PTR(self, err_self, "");

    // read args
    self->fd = va_arg(*app, int);
    self->context = va_arg(*app, void *);
    self->ident = va_arg(*app, void *);
    self->timeout_in_s = va_arg(*app, int);
    self->repeat = va_arg(*app, unsigned int);
    self->enabled = true;

    ASSERT_INT0(pthread_mutex_init(&self->mutex, NULL), err_mutex, "could not init mutex");

    return self;

err_mutex:
err_self:
    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcSocketJob_t *public_lfcSocketJob_dtor (
    lfcSocketJob_t *self
) {
    lfcObject_super_dtor(lfcSocketJob(), self);

    return self;
}

/**
 * Deaktiviert diesen Job, d.h. er wird beim Aufruf von <code>yourFDIsReady</code> oder <code>doWork</code> nicht arbeiten.
 *
 * @param self ist die Instanz
 * @return true .. konnte deaktiviert werden
 *          false .. konnte nicht deaktiviert werden
 */
static bool public_lfcSocketJob_disable (
    lfcSocketJob_t *self
) {
    bool result;

    pthread_mutex_lock(&self->mutex);
    self->enabled = false;
    result = self->enabled;
    pthread_mutex_unlock(&self->mutex);

    return !result;
}

/**
 * Aktiviert diesen Job, d.h. er wird beim Aufruf von <code>yourFDIsReady</code> oder <code>doWork</code> arbeiten.
 *
 * @param self ist die Instanz
 * @return true .. konnte aktiviert werden
 *          false .. konnte nicht aktiviert werden
 */
static bool public_lfcSocketJob_enable (
    lfcSocketJob_t *self
) {
    bool result;

    pthread_mutex_lock(&self->mutex);
    self->enabled = true;
    result = self->enabled;
    pthread_mutex_unlock(&self->mutex);

    return result;
}

/**
 * Gibt true zurück, wenn die4ser Job aktiviert ist.
 *
 * @param self ist die Instanz
 * @return true .. ist aktiviert
 *          false .. ist deaktiviert
 */
static bool public_lfcSocketJob_isEnabled (
    lfcSocketJob_t *self
) {
    bool result;

    pthread_mutex_lock(&self->mutex);
    result = self->enabled;
    pthread_mutex_unlock(&self->mutex);

    return result;
}

/**
 * Gibt den fd des Jobs zurueck.
 *
 * @param self Instanze-ptr
 * @return der fd
 */
static int public_lfcSocketJob_getFD(
    lfcSocketJob_t *self
) {
    return self->fd;
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
CLASS_CTOR__START(lfcSocketJob)
        OVERRIDE_METHOD(lfcSocketJob, enable)
        OVERRIDE_METHOD(lfcSocketJob, disable)
        OVERRIDE_METHOD(lfcSocketJob, isEnabled)

        OVERRIDE_METHOD(lfcSocketJob, getFD)

        OVERRIDE_METHOD(lfcSocketJob, doWork)
        OVERRIDE_METHOD(lfcSocketJob, reset)
    CLASS_CTOR__INIT_SUPER(lfcSocketJob, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcSocketJob_class_t *lfcSocketJob_class() {
    return _lfcSocketJob_class
           ? _lfcSocketJob_class
           : (_lfcSocketJob_class = (lfcSocketJob_class_t *) new(
            lfcObject_class(), "lfcSocketJob_class", lfcObject_class(), sizeof(lfcSocketJob_class_t),

            lfcObject_ctor, "", impl_lfcSocketJob_class_ctor,

            (void *) 0)
           );
}

const lfcSocketJob_t *lfcSocketJob() {
    return _lfcSocketJob
           ? _lfcSocketJob
           : (_lfcSocketJob = (lfcSocketJob_t *) new(
            lfcSocketJob_class(),
            "lfcSocketJob", lfcObject(), sizeof(lfcSocketJob_t),

            lfcObject_ctor, "ctor", public_lfcSocketJob_ctor,
            lfcObject_dtor, "dtor", public_lfcSocketJob_dtor,

            lfcSocketJob_disable, "disable", public_lfcSocketJob_disable,
            lfcSocketJob_enable, "enable", public_lfcSocketJob_enable,
            lfcSocketJob_isEnabled, "isEnabled", public_lfcSocketJob_isEnabled,

            lfcSocketJob_getFD, "getFD", public_lfcSocketJob_getFD,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcSocketJob);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

lfcOOP_accessor(lfcSocketJob, disable, bool);
lfcOOP_accessor(lfcSocketJob, enable, bool);
lfcOOP_accessor(lfcSocketJob, isEnabled, bool);

lfcOOP_accessor(lfcSocketJob, getFD, int);

lfcOOP_accessor(lfcSocketJob, doWork, int);
lfcOOP_accessor(lfcSocketJob, reset, bool);
