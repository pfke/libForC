#include "lfcQueue.h"

#include <stdlib.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcQueue_t *_lfcQueue;
static const lfcQueue_class_t *_lfcQueue_class;
static       lfcQueue_methods_t _lfcQueue_methods;

/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static lfcQueue_t *public_lfcQueue_ctor   (void *_self, va_list *app);
static lfcQueue_t *public_lfcQueue_dtor   (lfcQueue_t *self);
static int         public_lfcQueue_clear  (lfcQueue_t *self);
static int         public_lfcQueue_enqueue(lfcQueue_t *self, void *toAdd);
static void       *public_lfcQueue_dequeue(lfcQueue_t *self);

static size_t      impl_lfcQueue__IIterable__count    (lfcIIterable_t *_self);
static int         impl_lfcQueue__IIterable__exists   (lfcIIterable_t *_self, const void *toCheck);
static void       *impl_lfcQueue__IIterable__head     (lfcIIterable_t *_self);
static lfcQueue_t *impl_lfcQueue__IIterable__filter   (lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
static lfcQueue_t *impl_lfcQueue__IIterable__filterNot(lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
static void       *impl_lfcQueue__IIterable__find     (lfcIIterable_t *_self, lambda_fn__retInt__param1_voidPtr fn);
static void        impl_lfcQueue__IIterable__foreach  (lfcIIterable_t *_self, lambda_fn__noRet__param1_voidPtr fn);
static int         impl_lfcQueue__IIterable__isEmpty  (lfcIIterable_t *_self);
static void       *impl_lfcQueue__IIterable__last     (lfcIIterable_t *_self);
static lfcQueue_t *impl_lfcQueue__IIterable__map      (lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retVoidPtr__param1_voidPtr fn);
static int         impl_lfcQueue__IIterable__nonEmpty (lfcIIterable_t *_self);

/**
 * Constructor der Liste selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcQueue_t *public_lfcQueue_ctor(
    void *_self,
    va_list *app
) {
    lfcQueue_t *self = (lfcQueue_t *) lfcObject_super_ctor(lfcQueue(), _self);
    size_t table_size, increaseBy;

    if (self == 0)
        return 0;

    // read args
    table_size = va_arg(*app, size_t);
    increaseBy = va_arg(*app, size_t);

    self->table = lfcList_ctorWithSize(table_size, increaseBy);
    if (!self->table) {
        delete(self);
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
static lfcQueue_t *public_lfcQueue_dtor(
    lfcQueue_t *self
) {
    lfcObject_super_dtor(lfcQueue(), self);

    delete(self->table);
    pthread_mutex_destroy(&self->lock);

    return self;
}

/**
 * Löscht die Liste.
 *
 * @param _self die Instanz der Liste selbst
 */
static int public_lfcQueue_clear(
    lfcQueue_t *self
) {
    return lfcList_methods()->clear(self->table);
}

/**
 * Diese Methode fügt das angegebene Element an das Ende der Liste.
 *
 * @param _self die Instanz der Liste selbst
 * @param toAdd das hinzuzufügende Element
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcQueue_enqueue(
    lfcQueue_t *self,
    void *toAdd
) {
    return lfcList_methods()->add(self->table, toAdd);
}

/**
 * Gibt das Item an der angegebenen Position zurueck.
 *
 * @return NULL .. Bereichsüberschreitung
 *         >= 0 .. gefunden
 */
static void *public_lfcQueue_dequeue(
    lfcQueue_t *self
) {
    void *result = NULL;

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = lfcList_methods()->getAt(self->table, 0);
        lfcList_methods()->removeAt(self->table, 0);

        pthread_mutex_unlock(&self->lock);
    }

    return result;
}

/**
 * Gibt die Anzahl der enthaltenen Elemente zurueck.
 *
 * @param _self die Instanz der Liste selbst
 */
static size_t impl_lfcQueue__IIterable__count(
    lfcIIterable_t *_self
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return 0;


    return lfcList_methods()->lfcIIterable->count(self->table);
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
static int impl_lfcQueue__IIterable__exists(
    lfcIIterable_t *_self,
    const void *toCheck
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return 0;

    return lfcList_methods()->lfcIIterable->exists(self->table, toCheck);
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcQueue_t *impl_lfcQueue__IIterable__filter(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retInt__param1_voidPtr fn
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return 0;

    lfcQueue_t *result = lfcQueue_ctor();
    if (!result) return 0;

    lfcList_methods()->lfcIIterable->foreach(self->table, lambda(void, (void *iter isAnIncognito_param) {
        if (fn(iter)) {
        public_lfcQueue_enqueue(result, iter);
    }}));

    if (free_source == lfcifoc_FREE_OLD_COLLECION) {
        delete(self);
    }
    return result;
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcQueue_t *impl_lfcQueue__IIterable__filterNot(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retInt__param1_voidPtr fn
) {
    return impl_lfcQueue__IIterable__filter(_self, free_source, lambda(int, (void *item) { return !fn(item); }));
}

/**
 * Findet ein Element in der Liste
 */
static void *impl_lfcQueue__IIterable__find(
    lfcIIterable_t *_self,
    lambda_fn__retInt__param1_voidPtr fn
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return 0;

    void *result = NULL;

    lfcList_methods()->lfcIIterable->foreach(self->table, lambda(void, (void *iter isAnIncognito_param) {
        if (result == NULL && fn(iter)) {
        result = iter;
    }}));

    return result;
}

/**
 * Iteriert über die liste
 */
static void impl_lfcQueue__IIterable__foreach(
    lfcIIterable_t *_self,
    lambda_fn__noRet__param1_voidPtr fn
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return;

    lfcList_methods()->lfcIIterable->foreach(self->table, lambda(void, (void *iter isAnIncognito_param) {
        fn(iter);
    }));
}

/**
 * Gibt 1 zurueck, wenn die Liste leer ist
 *
 * @param _self die Instanz der Liste selbst
 */
static int impl_lfcQueue__IIterable__isEmpty(
    lfcIIterable_t *_self
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return 0;

    return lfcList_methods()->lfcIIterable->isEmpty(self->table);
}

/**
 * Gibt 1 zurueck, wenn die Liste nicht leer ist
 *
 * @param _self die Instanz der Liste selbst
 */
static int impl_lfcQueue__IIterable__nonEmpty(
    lfcIIterable_t *_self
) {
    return !impl_lfcQueue__IIterable__isEmpty(_self);
}

/**
 * Gibt das erste Element der Liste zurueck.
 */
static void *impl_lfcQueue__IIterable__head(
    lfcIIterable_t *_self __attribute__((unused))
) {
    return NULL;
}

/**
 * Gibt das erste Element der Liste zurueck.
 */
static void *impl_lfcQueue__IIterable__last(
    lfcIIterable_t *_self __attribute__((unused))
) {
    return NULL;
}

/**
 * Gibt eine neue gebaute Liste zurueck.
 */
static lfcQueue_t *impl_lfcQueue__IIterable__map(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retVoidPtr__param1_voidPtr fn
) {
    lfcQueue_t *self = asInstanceOf(lfcQueue(), _self);
    if (!self) return 0;

    lfcQueue_t *result = lfcQueue_ctor();

    lfcList_methods()->lfcIIterable->foreach(self->table, lambda(void, (void *iter isAnIncognito_param) {
        if (fn(iter)) {
        public_lfcQueue_enqueue(result, fn(iter));
    }}));

    if (free_source == lfcifoc_FREE_OLD_COLLECION) {
        delete(self);
    }
    return result;
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
CLASS_CTOR__START(lfcQueue)
        OVERRIDE_METHOD(lfcQueue, clear)
        OVERRIDE_METHOD(lfcQueue, enqueue)
        OVERRIDE_METHOD(lfcQueue, dequeue)
    CLASS_CTOR__INIT_SUPER(lfcQueue, lfcObject)
    CLASS_CTOR__INIT_IFACES()
        CLASS_IMPLEMENT_IFACE(lfcQueue, lfcIIterable)
CLASS_CTOR__END()

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
const lfcQueue_class_t *lfcQueue_class() {
    return _lfcQueue_class
           ? _lfcQueue_class
           : (_lfcQueue_class = (lfcQueue_class_t *) new(
            lfcObject_class(), "lfcQueue_class", lfcObject_class(), sizeof(lfcQueue_class_t),

            lfcObject_ctor, "", impl_lfcQueue_class_ctor,

            (void *) 0)
           );
}

const lfcQueue_t *lfcQueue() {
    return _lfcQueue
           ? _lfcQueue
           : (_lfcQueue = (lfcQueue_t *) new(
            lfcQueue_class(),
            "lfcQueue", lfcObject(), sizeof(lfcQueue_t),

            lfcObject_ctor, "ctor", public_lfcQueue_ctor,
            lfcObject_dtor, "dtor", public_lfcQueue_dtor,
            lfcQueue_clear, "clear", public_lfcQueue_clear,
            lfcQueue_enqueue, "enqueue", public_lfcQueue_enqueue,
            lfcQueue_dequeue, "dequeue", public_lfcQueue_dequeue,

            lfcIIterable_count, "count", impl_lfcQueue__IIterable__count,
            lfcIIterable_exists, "exists", impl_lfcQueue__IIterable__exists,
            lfcIIterable_filter, "filter", impl_lfcQueue__IIterable__filter,
            lfcIIterable_filterNot, "filterNot", impl_lfcQueue__IIterable__filterNot,
            lfcIIterable_find, "find", impl_lfcQueue__IIterable__find,
            lfcIIterable_foreach, "foreach", impl_lfcQueue__IIterable__foreach,
            lfcIIterable_isEmpty, "isEmpty", impl_lfcQueue__IIterable__isEmpty,
            lfcIIterable_head, "head", impl_lfcQueue__IIterable__head,
            lfcIIterable_last, "last", impl_lfcQueue__IIterable__last,
            lfcIIterable_map, "map", impl_lfcQueue__IIterable__map,
            lfcIIterable_nonEmpty, "nonEmpty", impl_lfcQueue__IIterable__nonEmpty,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcQueue);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Liste mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcQueue_t *lfcQueue_ctor(void) {
    return lfcQueue_ctorWithSize(DEFAULT_LIST_INITIAL_SIZE, DEFAULT_LIST_INCREASE_BY);
}

/**
 * Liste mit den angegebenen Größen initialisieren
 *
 * @param initial_size ist die initiale Größe der Liste
 * @param increase_by um diesen Wert wird die Liste bei Bedarf vergrößert
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcQueue_t *lfcQueue_ctorWithSize(
    size_t initial_size,
    size_t increase_by
) {
    return (lfcQueue_t *)new(lfcQueue(), initial_size, increase_by);
}

IMPL_API__wRET__w_0PARAM(lfcQueue, clear, int)
IMPL_API__wRET__w_1PARAM(lfcQueue, enqueue, int, void *)
IMPL_API__wRET__w_0PARAM(lfcQueue, dequeue, void *)
