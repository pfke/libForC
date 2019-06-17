#include "lfcBitset.h"

#include <stdlib.h>
#include <asm-generic/errno-base.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcBitset_t *_lfcBitset;
static const lfcBitset_class_t *_lfcBitset_class;
static lfcBitset_methods_t _lfcBitset_methods;

/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static lfcBitset_t *public_lfcBitset_ctor(void *_self, va_list *app);
static lfcBitset_t *public_lfcBitset_dtor(lfcBitset_t *self);
static int        public_lfcBitset_clear (lfcBitset_t *self, size_t index);
static size_t     public_lfcBitset_count (lfcBitset_t *self);
static int        public_lfcBitset_get   (lfcBitset_t *self, size_t index);
static int        public_lfcBitset_set   (lfcBitset_t *self, size_t index);
static int        public_lfcBitset_toggle(lfcBitset_t *self, size_t index);

/**
 * Constructor der Liste selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcBitset_t *public_lfcBitset_ctor(
    void *_self,
    va_list *app __attribute__((unused))
) {
    lfcBitset_t *self = (lfcBitset_t *) lfcObject_super_ctor(lfcBitset(), _self);

    if (self == 0)
        return 0;

    // read args
    self->array = NULL;
    self->array_count = 0;

    pthread_mutex_init(&self->lock, NULL);

    return self;
}

/**
 * Destructor der Liste selbst.
 */
static lfcBitset_t *public_lfcBitset_dtor(
    lfcBitset_t *self
) {
    lfcObject_super_dtor(lfcBitset(), self);

    if (self->array) {
        free(self->array);
    }
    self->array = NULL;
    self->array_count = 0;

    pthread_mutex_destroy(&self->lock);

    return self;
}

/**
 * Komplettes set leeren
 */
static int public_lfcBitset_clear(
    lfcBitset_t *self,
    size_t index
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        if (!self->array) {
            pthread_mutex_unlock(&self->lock);
            return 0;
        }

        self->array[index / 8] &= ~(1 << (index % 8));

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -1;
}

/**
 * Diese Methode gibt die Anzahl der Bits zurueck
 */
static size_t public_lfcBitset_count(
    lfcBitset_t *self
) {
    if (self->array == NULL) return 0;
    if (self->array_count == 0) return 0;

    if(0 == pthread_mutex_lock(&self->lock)) {
        size_t bit_count = 0;

        // TODO: genauer, wenn nötig
        bit_count = self->array_count * 8;

        pthread_mutex_unlock(&self->lock);
        return bit_count;
    }

    return 0;
}

/**
 * Bit an der Pos zurueckgeben
 */
static int public_lfcBitset_get(
    lfcBitset_t *self,
    size_t index
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        int result = 0;

        if (!self->array) {
            pthread_mutex_unlock(&self->lock);
            return 0;
        }

        result = 1 & (self->array[index / 8] >> (index % 8));

        pthread_mutex_unlock(&self->lock);
        return result;
    }

    return -1;
}

/**
 * Bit an der Pos setzen
 */
static int public_lfcBitset_set(
    lfcBitset_t *self,
    size_t index
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        size_t new_count = (index / 8) + 1;
        // ensure bits
        if (new_count > self->array_count) {
            char *new_ptr = realloc(self->array, sizeof(char) * new_count);

            if (new_ptr == NULL) {
                pthread_mutex_unlock(&self->lock);
                return -ENOMEM;
            }

            memset(&new_ptr[self->array_count], 0, sizeof(char) * (new_count - self->array_count));
            self->array = new_ptr;
            self->array_count = new_count;
        }

        if (self->array) {
            self->array[index / 8] |= 1 << (index % 8);
            pthread_mutex_unlock(&self->lock);
            return -ENOMEM;
	}

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -1;
}

/**
 * Bit an der Pos toggeln
 */
static int public_lfcBitset_toggle(
    lfcBitset_t *self,
    size_t index
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        // ensure bits
        if ((index / 8) >= self->array_count) {
            size_t new_count = (index / 8) + 1;

            char *new_ptr = realloc(self->array, sizeof(char) * new_count);

            if (self->array == new_ptr) {
                pthread_mutex_unlock(&self->lock);
                return -ENOMEM;
            }

            memset(&new_ptr[self->array_count], 0, sizeof(char) * (new_count - self->array_count));
            self->array = new_ptr;
            self->array_count = new_count;
        }

        if (self->array) {
            self->array[index / 8] ^= 1 << (index % 8);
        }

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -1;
}


/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcBitset)
        OVERRIDE_METHOD(lfcBitset, clear)
        OVERRIDE_METHOD(lfcBitset, count)
        OVERRIDE_METHOD(lfcBitset, get)
        OVERRIDE_METHOD(lfcBitset, set)
        OVERRIDE_METHOD(lfcBitset, toggle)
    CLASS_CTOR__INIT_SUPER(lfcBitset, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
const lfcBitset_class_t *lfcBitset_class() {
    return _lfcBitset_class
           ? _lfcBitset_class
           : (_lfcBitset_class = (lfcBitset_class_t *) new(
            lfcObject_class(), "lfcBitset_class", lfcObject_class(), sizeof(lfcBitset_class_t),

            lfcObject_ctor, "", impl_lfcBitset_class_ctor,

            (void *) 0)
           );
}

const lfcBitset_t *lfcBitset() {
    return _lfcBitset
           ? _lfcBitset
           : (_lfcBitset = (lfcBitset_t *) new(
            lfcBitset_class(),
            "lfcBitset", lfcObject(), sizeof(lfcBitset_t),

            lfcObject_ctor, "ctor", public_lfcBitset_ctor,
            lfcObject_dtor, "dtor", public_lfcBitset_dtor,
            lfcBitset_clear, "clear", public_lfcBitset_clear,
            lfcBitset_count, "count", public_lfcBitset_count,
            lfcBitset_get, "get", public_lfcBitset_get,
            lfcBitset_set, "set", public_lfcBitset_set,
            lfcBitset_toggle, "toggle", public_lfcBitset_toggle,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcBitset);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Bitset initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcBitset_t *lfcBitset_ctor() {
    return (lfcBitset_t *)new(lfcBitset());
}

/**
 * Bitset initialisieren (und die uebergebenen Bits gleich setzen).
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcBitset_t *lfcBitset_ctor_withBits(
    size_t argc,
    ...
) {
    va_list ap;
    lfcBitset_t *self;

    self = lfcBitset_ctor();
    if (!self) return NULL;

    va_start(ap, argc);
    while(argc--) {
        lfcBitset_set(self, va_arg(ap, size_t));
    }
    va_end(ap);

    return self;
}

lfcOOP_accessor(lfcBitset, clear, int, size_t)
lfcOOP_accessor(lfcBitset, count, size_t)
lfcOOP_accessor(lfcBitset, get, int, size_t)
lfcOOP_accessor(lfcBitset, set, int, size_t)
lfcOOP_accessor(lfcBitset, toggle, int, size_t)
