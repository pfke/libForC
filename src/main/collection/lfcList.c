#include "lfcList.h"

#include <stdlib.h>
#include <asm-generic/errno-base.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcList_t *_lfcList;
static const lfcList_class_t *_lfcList_class;
static lfcList_methods_t _lfcList_methods;

/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static int private_lfcList_bubbleSort (lfcList_t *self, size_t n, lfcList_compare_fn compare_fn);

static lfcList_t *public_lfcList_ctor    (void *_self, va_list *app);
static lfcList_t *public_lfcList_dtor    (lfcList_t *self);
static int        public_lfcList_add     (lfcList_t *self, void *toAdd);
static int        public_lfcList_clear   (lfcList_t *self);
static void      *public_lfcList_getAt   (lfcList_t *self, size_t idx);
static size_t     public_lfcList_indexOf (lfcList_t *self, const void *toCheck);
static int        public_lfcList_insertAt(lfcList_t *self, size_t idx, void *toAdd);
static int        public_lfcList_remove  (lfcList_t *self, const void *toRemove);
static int        public_lfcList_removeAt(lfcList_t *self, size_t idx);

static int public_lfcList_bubbleSort (lfcList_t *self, lfcList_compare_fn compare_fn);

static size_t     impl_lfcList__IIterable__count    (lfcIIterable_t *_self);
static int        impl_lfcList__IIterable__exists   (lfcIIterable_t *_self, const void *toCheck);
static void      *impl_lfcList__IIterable__head     (lfcIIterable_t *_self);
static lfcList_t *impl_lfcList__IIterable__filter   (lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
static lfcList_t *impl_lfcList__IIterable__filterNot(lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
static void      *impl_lfcList__IIterable__find     (lfcIIterable_t *_self, lambda_fn__retInt__param1_voidPtr fn);
static void       impl_lfcList__IIterable__foreach  (lfcIIterable_t *_self, lambda_fn__noRet__param1_voidPtr fn);
static int        impl_lfcList__IIterable__isEmpty  (lfcIIterable_t *_self);
static void      *impl_lfcList__IIterable__last     (lfcIIterable_t *_self);
static lfcList_t *impl_lfcList__IIterable__map      (lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retVoidPtr__param1_voidPtr fn);
static void      *impl_lfcList__IIterable__next     (lfcIIterable_t *_self, const void *current);
static int        impl_lfcList__IIterable__nonEmpty (lfcIIterable_t *_self);

/**
 * Der Sortier-Algorithmus selber
 */
static int private_lfcList_bubbleSort (
    lfcList_t *self,
    size_t n,
    lfcList_compare_fn compare_fn
) {
    size_t i;

    // Base case
    if (n == 0) { return 0; }
    if (n == 1) { return 0; }

    // One pass of bubble sort. After
    // this pass, the largest element
    // is moved (or bubbled) to end.
    for (i = 0; i < n-1; i++) {
        void *a = self->table[i];
        void *b = self->table[i + 1];

        if (compare_fn(a, b) > 0) {
            // swap die scheisse
            self->table[i] = b;
            self->table[i + 1] = a;
        }
    }

    // Largest element is fixed,
    // recur for remaining array
    return private_lfcList_bubbleSort(self, n - 1, compare_fn);
}

/**
 * Diese Methode stellt sicher, dass die angeforderte Größe hier vergügbar ist.
 * Es wird erwartet, dass der Lock gesetzt ist.
 *
 * @param self ist unsere Instanz
 * @param size_to_ensure die Tabelle soll min. diese Größe haben
 * @return 0 .. alles ok
 *          !0 .. nichts ist ok
 */
static int private_lfcList_ensureSize (
    lfcList_t *self,
    size_t size_to_ensure
) {
    if (size_to_ensure > self->table_size) {
        size_t increased_by = lfcMAX(size_to_ensure - self->table_size, self->increaseBy);

        void ** ptr = (void **)realloc(self->table, (self->table_size + increased_by) * sizeof(void *));

        if(!ptr) {
            return -ENOMEM;
        }
        self->table_size += increased_by;
        memset(ptr + self->size, 0, increased_by * sizeof(void *));

        self->table = ptr;
    }

    return 0;
}

/**
 * Die Liste soll verkleinert werden. Mal Schauen ob das sinnvoll ist.
 *
 * Wenn die Anzahl der freien Einträge <code>self->increaseBy * 3</code> übersteigt, wird auf <code>self->size + self->increaseBy</code>
 * verkleinert.
 *
 * Es wird erwartet, dass der Lock gesetzt ist.
 *
 * @param self
 * @return 0 .. ok
 *          !0 .. nicht ok
 */
static int private_lfcList_shrink (
    lfcList_t *self
) {
    if ((self->table_size - self->size) > (self->increaseBy * 3)) {
        size_t decrease_to = self->size + self->increaseBy;

        void ** ptr = (void **)realloc(self->table, decrease_to * sizeof(void *));

        if(!ptr) {
            return -ENOMEM;
        }
        self->table_size = decrease_to;
        self->table = ptr;
    }

    return 0;
}

/**
 * Constructor der Liste selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcList_t *public_lfcList_ctor(
    void *_self,
    va_list *app
) {
    lfcList_t *self = (lfcList_t *) lfcObject_super_ctor(lfcList(), _self);

    if (self == 0)
        return 0;

    // read args
    self->table_size = va_arg(*app, size_t);
    self->increaseBy = va_arg(*app, size_t);

    self->size = 0;
    self->table = (void **)calloc(self->table_size, sizeof(void *));
    if (!self->table) {
        goto err_no_mem;
    }

    pthread_mutex_init(&self->lock, NULL);

    return self;

    err_no_mem:
    delete(self);

    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcList_t *public_lfcList_dtor(
    lfcList_t *self
) {
    pthread_mutex_lock(&self->lock);
    pthread_mutex_unlock(&self->lock);
    pthread_mutex_destroy(&self->lock);

    free(self->table);
    self->table = 0;
    self->increaseBy = 0;
    self->table_size = 0;

    return lfcObject_super_dtor(lfcList(), self);
}

/**
 * Diese Methode fügt das angegebene Element der Liste hinzu.
 *
 * @param _self die Instanz der Liste selbst
 * @param toAdd das hinzuzufügende Element
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcList_add(
    lfcList_t *self,
    void *toAdd
) {
    return public_lfcList_insertAt(self, self->size, toAdd);
}

/**
 * Löscht die Liste.
 *
 * @param _self die Instanz der Liste selbst
 */
static int public_lfcList_clear(
    lfcList_t *self
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        self->size = 0;

        // liste verkleinern
        private_lfcList_shrink(self);

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -1;
}

/**
 * Gibt das Item an der angegebenen Position zurueck.
 *
 * @return NULL .. Bereichsüberschreitung
 *         >= 0 .. gefunden
 */
static void *public_lfcList_getAt(
    lfcList_t *self,
    size_t idx
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        void *ret = NULL;
        if (idx < self->size) {
            ret = self->table[idx];
        }
        pthread_mutex_unlock(&self->lock);
        return ret;
    }

    return NULL;
}

/**
 * Gibt den Index des übergebenen Objektes zurück, wenn es in der Liste ist.
 *
 * @param _self die Instanz der Liste selbst
 * @param toCheck das zu findende Element
 *
 * @return UINT32_MAX .. nicht gefunden
 *         >= 0 .. gefunden
 */
static size_t public_lfcList_indexOf(
    lfcList_t *self,
    const void *toCheck
) {
    uint32_t i;

    for (i=0; i<self->size; i++) {
        if (public_lfcList_getAt(self, i) == toCheck) {
            return i;
        }
    }

    return UINT32_MAX;
}

/**
 * Fügt ein Element an der angegebenen Postition ein.
 *
 * @param self die Instanz der Liste selbst
 * @param idx position an der das Element eingefügt werden soll
 * @param toAdd ist dfas hinzuzufügende Element
 *
 * @return 0 .. ok
 *         -EACCESS .. mutex konnte nicht bezogen werden
 */
static int public_lfcList_insertAt(
    lfcList_t *self,
    size_t idx,
    void *toAdd
) {
    if (toAdd == NULL) { return -EINVAL; }

    if(0 == pthread_mutex_lock(&self->lock)) {
        if (private_lfcList_ensureSize(self, self->size + 1)) { // +1 wg. 0-Index
            pthread_mutex_unlock(&self->lock);
            return -ENOMEM;
        }

        if (idx < self->size) {
            memmove(&self->table[idx + 1], &self->table[idx], (self->size - idx) * sizeof(void *));
        }
        self->table[idx] = toAdd;
        self->size++;

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -EACCES;
}

/**
 * Diese Methode entfernt das angegebene Element von der Liste.
 *
 * @param _self die Instanz der Liste selbst
 * @param toRemove das zu löschende Element
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcList_remove(
    lfcList_t *self,
    const void *toRemove
) {
    return public_lfcList_removeAt(self, public_lfcList_indexOf(self, toRemove));
}

/**
 * Diese Methode entfernt das Element von der Liste an der angegebenen Position.
 *
 * @param _self die Instanz der Liste selbst
 * @param idx is die Position des zu löschenden Elementes
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcList_removeAt(
    lfcList_t *self,
    size_t idx
) {
    if(idx >= self->size) {
        return -1;
    }

    if(0 == pthread_mutex_lock(&self->lock)) {
        size_t i;
        for(i = idx; i < self->size; i++) {
            self->table[i] = self->table[i + 1];
        }

        self->table[self->size - 1] = 0;
        self->size--;

        // ggf. verkleinern
        private_lfcList_shrink(self);

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -1;
}

/**
 * Diese Methode entfernt das Element von der Liste an der angegebenen Position.
 *
 * @param _self die Instanz der Liste selbst
 * @param idx is die Position des zu löschenden Elementes
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcList_bubbleSort (
    lfcList_t *self,
    lfcList_compare_fn compare_fn
) {
    if (!compare_fn) return -1;

    if(0 == pthread_mutex_lock(&self->lock)) {
        private_lfcList_bubbleSort(self, impl_lfcList__IIterable__count((lfcIIterable_t *)self), compare_fn);

        pthread_mutex_unlock(&self->lock);
        return 0;
    }

    return -1;
}

/**
 * Gibt die Anzahl der enthaltenen Elemente zurueck.
 *
 * @param _self die Instanz der Liste selbst
 */
static size_t impl_lfcList__IIterable__count(
    lfcIIterable_t *_self
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);
    if (!self) return 0;

    return self->size;
}

/**
 * Gibt 1 zurueck, falls da Item in der Liste existiert.
 *
 * @param _self die Instanz der Liste selbst
 * @param toCheck das zu findende Element
 *
 * @return 1 .. gefunden
 *         0 .. nicht gefunden
 */
static int impl_lfcList__IIterable__exists(
    lfcIIterable_t *_self,
    const void *toCheck
) {
    uint32_t i;
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;

    for (i=0; i<self->size; i++) {
        if (public_lfcList_getAt(self, i) == toCheck) {
            return 1;
        }
    }

    return 0;
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcList_t *impl_lfcList__IIterable__filter(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retInt__param1_voidPtr fn
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;
    if (!fn) return 0;

    uint32_t i;
    lfcList_t *result = lfcList_ctor();
    if (!result) return NULL;

    for (i=0; i<self->size; i++) {
        if (!fn(self->table[i])) {
            continue;
        }

        public_lfcList_add(result, self->table[i]);
    }

    if (free_source == lfcifoc_FREE_OLD_COLLECION) {
        delete(self);
    }
    return result;
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcList_t *impl_lfcList__IIterable__filterNot(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retInt__param1_voidPtr fn
) {
    if (!fn) return 0;

    return impl_lfcList__IIterable__filter(_self, free_source, lambda(int, (void *item) { return !fn(item); }));
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static void *impl_lfcList__IIterable__find(
    lfcIIterable_t *_self,
    lambda_fn__retInt__param1_voidPtr fn
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;
    if (!fn) return 0;

    uint32_t i;
    void *result = NULL;

    for (i=0; i<self->size; i++) {
        if (!fn(self->table[i])) {
            continue;
        }

        result = self->table[i];
        break;
    }

    return result;
}

/**
 * Iteriert über die liste
 */
static void impl_lfcList__IIterable__foreach(
    lfcIIterable_t *_self,
    lambda_fn__noRet__param1_voidPtr fn
) {
    uint32_t i;
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return;
    if (!fn) return;

    for (i = 0; i < self->size; i++) {
        fn(self->table[i]);
    }
}

/**
 * Gibt 1 zurueck, wenn die Liste leer ist
 *
 * @param _self die Instanz der Liste selbst
 */
static int impl_lfcList__IIterable__isEmpty(
    lfcIIterable_t *_self
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;

    return self->size == 0;
}

/**
 * Gibt 1 zurueck, wenn die Liste nicht leer ist
 *
 * @param _self die Instanz der Liste selbst
 */
static int impl_lfcList__IIterable__nonEmpty(
    lfcIIterable_t *_self
) {
    return !impl_lfcList__IIterable__isEmpty(_self);
}

/**
 * Gibt das erste Element der Liste zurueck.
 */
static void *impl_lfcList__IIterable__head(
    lfcIIterable_t *_self
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;

    return (self->size > 0) ? self->table[0] : 0;
}

/**
 * Gibt das erste Element der Liste zurueck.
 */
static void *impl_lfcList__IIterable__last(
    lfcIIterable_t *_self
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;

    return (self->size > 0) ? self->table[self->size - 1] : 0;
}

/**
 * Gibt eine neue gebaute Liste zurueck.
 */
static lfcList_t *impl_lfcList__IIterable__map(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retVoidPtr__param1_voidPtr fn
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);

    if (!self) return 0;
    if (!fn) return 0;

    uint32_t i;
    lfcList_t *result = lfcList_ctor();
    if (!result) return NULL;

    for (i=0; i<self->size; i++) {
        public_lfcList_add(result, fn(self->table[i]));
    }

    if (free_source == lfcifoc_FREE_OLD_COLLECION) {
        delete(self);
    }
    return result;
}

/**
 * Gibt eine neue gebaute Liste zurueck.
 */
static void *impl_lfcList__IIterable__next(
    lfcIIterable_t *_self,
    const void *current
) {
    lfcList_t *self = asInstanceOf(lfcList(), _self);
    size_t current_item;

    if (!self) return 0;

    current_item = public_lfcList_indexOf(self, current);

    return (current_item == UINT32_MAX)
        ? lfcIIterable_head(self)
        : public_lfcList_getAt(self, current_item + 1);
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
CLASS_CTOR__START(lfcList)
        OVERRIDE_METHOD(lfcList, add)
        OVERRIDE_METHOD(lfcList, clear)
        OVERRIDE_METHOD(lfcList, getAt)
        OVERRIDE_METHOD(lfcList, indexOf)
        OVERRIDE_METHOD(lfcList, insertAt)
        OVERRIDE_METHOD(lfcList, remove)
        OVERRIDE_METHOD(lfcList, removeAt)

        OVERRIDE_METHOD(lfcList, bubbleSort)
    CLASS_CTOR__INIT_SUPER(lfcList, lfcObject)
    CLASS_CTOR__INIT_IFACES()
        CLASS_IMPLEMENT_IFACE(lfcList, lfcIIterable)
CLASS_CTOR__END()

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
const lfcList_class_t *lfcList_class() {
    return _lfcList_class
           ? _lfcList_class
           : (_lfcList_class = (lfcList_class_t *) new(
            lfcObject_class(), "lfcList_class", lfcObject_class(), sizeof(lfcList_class_t),

            lfcObject_ctor, "", impl_lfcList_class_ctor,

            (void *) 0)
           );
}

const lfcList_t *lfcList() {
    return _lfcList
           ? _lfcList
           : (_lfcList = (lfcList_t *) new(
            lfcList_class(),
            "lfcList", lfcObject(), sizeof(lfcList_t),

            lfcObject_ctor, "ctor", public_lfcList_ctor,
            lfcObject_dtor, "dtor", public_lfcList_dtor,
            lfcList_add, "add", public_lfcList_add,
            lfcList_clear, "clear", public_lfcList_clear,
            lfcList_getAt, "getAt", public_lfcList_getAt,
            lfcList_indexOf, "indexOf", public_lfcList_indexOf,
            lfcList_insertAt, "insertAt", public_lfcList_insertAt,
            lfcList_remove, "remove", public_lfcList_remove,
            lfcList_removeAt, "removeAt", public_lfcList_removeAt,

            lfcList_bubbleSort, "bubbleSort", public_lfcList_bubbleSort,

            lfcIIterable_count, "count", impl_lfcList__IIterable__count,
            lfcIIterable_exists, "exists", impl_lfcList__IIterable__exists,
            lfcIIterable_filter, "filter", impl_lfcList__IIterable__filter,
            lfcIIterable_filterNot, "filterNot", impl_lfcList__IIterable__filterNot,
            lfcIIterable_find, "find", impl_lfcList__IIterable__find,
            lfcIIterable_foreach, "foreach", impl_lfcList__IIterable__foreach,
            lfcIIterable_isEmpty, "isEmpty", impl_lfcList__IIterable__isEmpty,
            lfcIIterable_head, "head", impl_lfcList__IIterable__head,
            lfcIIterable_last, "last", impl_lfcList__IIterable__last,
            lfcIIterable_map, "map", impl_lfcList__IIterable__map,
            lfcIIterable_next, "next", impl_lfcList__IIterable__next,
            lfcIIterable_nonEmpty, "nonEmpty", impl_lfcList__IIterable__nonEmpty,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcList);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Liste mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcList_t *lfcList_ctor() {
    return lfcList_ctorWithSize(DEFAULT_LIST_INITIAL_SIZE, DEFAULT_LIST_INCREASE_BY);
}

/**
 * Liste mit den angegebenen Größen initialisieren
 *
 * @param initial_size ist die initiale Größe der Liste
 * @param increase_by um diesen Wert wird die Liste bei Bedarf vergrößert
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcList_t *lfcList_ctorWithSize(
    size_t initial_size,
    size_t increase_by
) {
    return (lfcList_t *)new(lfcList(), initial_size, increase_by);
}

lfcOOP_IMPL_ACCESSOR(lfcList, add, int, void *)
lfcOOP_IMPL_ACCESSOR(lfcList, bubbleSort, int, lfcList_compare_fn)
lfcOOP_IMPL_ACCESSOR(lfcList, clear, int)
lfcOOP_IMPL_ACCESSOR(lfcList, getAt, void *, size_t)
lfcOOP_IMPL_ACCESSOR(lfcList, indexOf, size_t, const void *)
lfcOOP_IMPL_ACCESSOR(lfcList, insertAt, int, size_t, void *)
lfcOOP_IMPL_ACCESSOR(lfcList, remove, int, const void *)
lfcOOP_IMPL_ACCESSOR(lfcList, removeAt, int, size_t)
