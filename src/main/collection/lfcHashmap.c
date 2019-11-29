#include "lfcHashmap.h"

#include <stdlib.h>

#define MAP_IN_USE_MAGIC      0x1F

#define MAP_MISSING   -3  // No such element
#define MAP_FULL      -2  // Hashmap is full
#define MAP_OMEM      -1  // Out of Memory
#define MAP_OK         0  // OK

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcHashmap_t *_lfcHashmap;
static const lfcHashmap_class_t *_lfcHashmap_class;
static lfcHashmap_methods_t _lfcHashmap_methods;

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/
static int priv_lfcHashmap_hash  (lfcHashmap_t *self, const void *key);
static int priv_lfcHashmap_put   (lfcHashmap_t *self, const void *key, void *value);
static int priv_lfcHashmap_rehash(lfcHashmap_t *self);

/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static lfcHashmap_t *public_lfcHashmap_ctor   (lfcHashmap_t *self, va_list *app);
static lfcHashmap_t *public_lfcHashmap_dtor   (lfcHashmap_t *self);
static int           public_lfcHashmap_clear  (lfcHashmap_t *self);
static void         *public_lfcHashmap_get    (lfcHashmap_t *self, const void *key);
static int           public_lfcHashmap_put    (lfcHashmap_t *self, const void *key, void *value);
static int           public_lfcHashmap_puto   (lfcHashmap_t *self, FILE *fp);
static int           public_lfcHashmap_remove (lfcHashmap_t *self, const void *key);

static size_t                     impl_lfcHashmap__IIterable__count    (lfcIIterable_t *_self);
static int                        impl_lfcHashmap__IIterable__exists   (lfcIIterable_t *_self, const void *key);
static void                      *impl_lfcHashmap__IIterable__head     (lfcIIterable_t *_self);
static lfcHashmap_t              *impl_lfcHashmap__IIterable__filter   (lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
static lfcHashmap_t              *impl_lfcHashmap__IIterable__filterNot(lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
static lfcHashmap_keyValuePair_t *impl_lfcHashmap__IIterable__find     (lfcIIterable_t *_self, lambda_fn__retInt__param1_voidPtr fn);
static void                       impl_lfcHashmap__IIterable__foreach  (lfcIIterable_t *_self, lambda_fn__noRet__param1_voidPtr fn);
static int                        impl_lfcHashmap__IIterable__isEmpty  (lfcIIterable_t *_self);
static void                      *impl_lfcHashmap__IIterable__last     (lfcIIterable_t *_self);
static lfcHashmap_t              *impl_lfcHashmap__IIterable__map      (lfcIIterable_t *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retVoidPtr__param1_voidPtr fn);
static int                        impl_lfcHashmap__IIterable__nonEmpty (lfcIIterable_t *_self);

/*
 * Hashing function for an integer
 */
unsigned int lfcHashmap_hash_int(
    lfcHashmap_t *self,
    const void *key
) {
    int result = (int)(intptr_t)key;

    // Robert Jenkins' 32 bit Mix Function
    result += (result << 12);
    result ^= (result >> 22);
    result += (result << 4);
    result ^= (result >> 9);
    result += (result << 10);
    result ^= (result >> 2);
    result += (result << 7);
    result ^= (result >> 12);

    // Knuth's Multiplicative Method
    result = (unsigned int)((result >> 3) * 2654435761u);

    return (unsigned int)(result % self->table_size);
}

/*
 * Hashing function for a ptr
 */
unsigned int lfcHashmap_hash_ptr (
    lfcHashmap_t *self,
    const void *key
) {
    return lfcHashmap_hash_int(self, key);
}

/*
 * Hashing function for a string
 */
unsigned int lfcHashmap_hash_string (
    lfcHashmap_t *self,
    const void *key
) {
    const char *in = (const char *)key;
    unsigned  result = (unsigned )*in;

    for(; *in; ++in) {
        result = (result << 5) - result + *in;
    }
    result = result % (int)self->table_size;

    return result;
}

/*
 * Return the integer of the location in data
 * to store the point to the item, or MAP_FULL.
 */
static int priv_lfcHashmap_hash(
    lfcHashmap_t *self,
    const void *key
){
    int curr;
    size_t i;

    // If full, return immediately
    if(self->size == self->table_size) return MAP_FULL;

    // Find the best index
    curr = self->hash_fn(self, key);

    // Linear probling
    for(i = 0; i< self->table_size; i++){
        lfcHashmap_element_t *item = &self->table[curr];

        if(item->in_use == 0)
            return curr;

        if(item->key == key)
            return curr;

        curr = (curr + 1) % (int)self->table_size;
    }

    return MAP_FULL;
}

/**
 * Diese Methode fügt das angegebene Element der map hinzu.
 *
 * @param _self die Instanz der Liste selbst
 * @param toAdd das hinzuzufügende Element
 *
 * @return 0 .. hat geklappt
 */
static int priv_lfcHashmap_put(
    lfcHashmap_t *self,
    const void *key,
    void *value
) {
    // Find a place to put our value
    int index = priv_lfcHashmap_hash(self, key);

    while(index == MAP_FULL) {
        if (priv_lfcHashmap_rehash(self) == MAP_OMEM) {
            pthread_mutex_unlock(&self->lock);
            return MAP_OMEM;
        }
        index = priv_lfcHashmap_hash(self, key);
    }

    // Set the data
    lfcHashmap_element_t *item = &self->table[index];

    item->in_use = MAP_IN_USE_MAGIC;
    item->data = value;
    item->key = key;
    self->size++;

    return MAP_OK;
}

/*
 * Doubles the size of the hashmap, and rehashes all the elements
 */
static int priv_lfcHashmap_rehash(
    lfcHashmap_t *self
){
    size_t i;
    size_t old_size;
    lfcHashmap_element_t *curr;

    // Setup the new elements
    lfcHashmap_element_t * temp = (lfcHashmap_element_t *)calloc(2 * self->table_size, sizeof(lfcHashmap_element_t));
    if(!temp) return MAP_OMEM;

    // Update the array
    curr = self->table;
    self->table = temp;

    // Update the size
    old_size = self->table_size;
    self->table_size = 2 * self->table_size;
    self->size = 0;

    // Rehash the elements
    for(i = 0; i < old_size; i++) {
        int status = priv_lfcHashmap_put(self, curr[i].key, curr[i].data);

        if (status != MAP_OK) {
            return status;
        }
    }
    free(curr);

    return MAP_OK;
}

/**
 * Constructor der Map selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcHashmap_t *public_lfcHashmap_ctor(
    lfcHashmap_t *self,
    va_list *app
) {
    lfcObject_super_ctor(lfcHashmap(), self);

    if (self == 0)
        return 0;

    // read args
    self->table_size = va_arg(*app, size_t);
    self->hash_fn = va_arg(*app, lfcHashmap_hash_fn);

    // create initial table
    if (self->table_size == 0) self->table_size = DEFAULT_HASHMAP_INITIAL_SIZE;
    self->table = calloc(self->table_size, sizeof(lfcHashmap_element_t));
    if (self->table == 0)
        return 0;

    pthread_mutex_init(&self->lock, NULL);

    return self;
}

/**
 * Destructor der Map selbst.
 */
static lfcHashmap_t *public_lfcHashmap_dtor(
    lfcHashmap_t *self
) {
    lfcObject_super_dtor(lfcHashmap(), self);

    free(self->table);
    pthread_mutex_destroy(&self->lock);

    return self;
}

/**
 * Löscht die Liste.
 *
 * @param _self die Instanz der Liste selbst
 */
static int public_lfcHashmap_clear(
    lfcHashmap_t *self
) {
    // Lock for concurrency
    if(0 == pthread_mutex_lock(&self->lock)) {
        memset(self->table, 0, self->table_size * sizeof(lfcHashmap_element_t));
        self->size = 0;

        // Unlock
        pthread_mutex_unlock(&self->lock);
        return 0;
    }
    return -1;
}

/**
 * Gibt die keys als Liste zurueck
 */
static void *public_lfcHashmap_get(
    lfcHashmap_t *self,
    const void *key
) {
    // Lock for concurrency
    if(0 == pthread_mutex_lock(&self->lock)) {
        size_t i;
        int curr;

        // Find data location
        curr = self->hash_fn(self, key);

        /* Linear probing, if necessary */
        for(i = 0; i < self->table_size; i++) {
            lfcHashmap_element_t *item = &self->table[curr];

            if(item->key == key && item->in_use == MAP_IN_USE_MAGIC) {
                void *result = item->data;

                pthread_mutex_unlock(&self->lock);
                return result;
            }

            curr = (curr + 1) % (int)self->table_size;
        }

        // Unlock
        pthread_mutex_unlock(&self->lock);
    }

    // Not found
    return 0;
}

/**
 * Diese Methode fügt das angegebene Element der map hinzu.
 *
 * @param _self die Instanz der Liste selbst
 * @param toAdd das hinzuzufügende Element
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcHashmap_put(
    lfcHashmap_t *self,
    const void *key,
    void *value
) {
    int result = MAP_OMEM;

    // Lock for concurrency
    if(0 == pthread_mutex_lock(&self->lock)) {
        result = priv_lfcHashmap_put(self, key, value);

        // Unlock
        pthread_mutex_unlock(&self->lock);
    }

    return result;
}

/**
 * Print some basic oop infos.
 *
 * @param _self is the object itself
 *
 * @return the fprintf return value
 */
static int public_lfcHashmap_puto (
    lfcHashmap_t *self,
    FILE *fp
) {
    size_t i;
    char buf[100];

    fprintf(fp, "==================\n");
    fprintf(fp, "hashmap (#%zu/%zu)\n", self->size, self->table_size);

    for(i = 0; i < self->table_size; i++) {
        lfcHashmap_element_t *item = &self->table[i];

        fprintf(fp, "%d\n", __LINE__);
        if (item->in_use == MAP_IN_USE_MAGIC) {
            memset(buf, 0, sizeof(buf));

            if (self->hash_fn == lfcHashmap_hash_int) {
                snprintf(buf, sizeof(buf) - 1, "%u", (int)(intptr_t)item->key);
            } else if (self->hash_fn == lfcHashmap_hash_ptr) {
                snprintf(buf, sizeof(buf) - 1, "%p", item->key);
            } else if (self->hash_fn == lfcHashmap_hash_string) {
                snprintf(buf, sizeof(buf) - 1, "%s", (char *)item->key);
            } else {
                snprintf(buf, sizeof(buf) - 1, "%p", item->key);
            }

//      if (isInstanceOf(lfcObject(), item)) {
//        // TODO: tostring method
//        fprintf(fp, "  #%lu key: '%s', value: 'TODO: tostring'\n", i, buf);
//      } else {
            fprintf(fp, "  #%zu key: '%s', value: '%p'\n", i, buf, item->data);
//      };
        } else {
            fprintf(fp, "  #%zu ---\n", i);
        }
    }

    fprintf(fp, "==================\n");

    return 0;
}

/**
 * Diese Methode entfernt das angegebene Element von der Liste.
 *
 * @param _self die Instanz der Liste selbst
 * @param toRemove das zu löschende Element
 *
 * @return 0 .. hat geklappt
 */
static int public_lfcHashmap_remove(
    lfcHashmap_t *self,
    const void *key
) {
    size_t i;

    // Lock for concurrency
    if(0 == pthread_mutex_lock(&self->lock)) {
        // Find key
        int curr = self->hash_fn(self, key);

        // Linear probing, if necessary
        for(i = 0; i < self->table_size; i++) {
            lfcHashmap_element_t *item = &self->table[curr];

            if(item->key == key && item->in_use == MAP_IN_USE_MAGIC) {
                // Blank out the fields
                item->in_use = 0;
                item->data = NULL;
                item->key = 0;

                /* Reduce the size */
                self->size--;
                pthread_mutex_unlock(&self->lock);
                return MAP_OK;
            }
            curr = (curr + 1) % (int)self->table_size;
        }

        // Unlock
        pthread_mutex_unlock(&self->lock);
    }

    // Data not found
    return MAP_MISSING;
}

/**
 * Diese Methode liefert alle values als Liste zurück.
 *
 * @param _self die Instanz der Liste selbst
 *
 * @return die neue Liste
 */
static lfcList_t *public_lfcHashmap_values(
    lfcHashmap_t *self
) {
    size_t i;
    lfcList_t *list = lfcList_ctor();

    // Lock for concurrency
    if(0 == pthread_mutex_lock(&self->lock)) {
        // Linear probing, if necessary
        for(i = 0; i < self->table_size; i++) {
            lfcHashmap_element_t *item = &self->table[i];

            if (item->in_use != MAP_IN_USE_MAGIC) {
                continue;
            }

            lfcList_add(list, item->data);
        }

        // Unlock
        pthread_mutex_unlock(&self->lock);
    }

    // Data not found
    return list;
}

/**
 * Gibt die Anzahl der enthaltenen Elemente zurueck.
 *
 * @param _self die Instanz der Liste selbst
 */
static size_t impl_lfcHashmap__IIterable__count(
    lfcIIterable_t *_self
) {
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
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
static int impl_lfcHashmap__IIterable__exists(
    lfcIIterable_t *_self,
    const void *key
) {
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
    if (!self) return 0;

    return public_lfcHashmap_get(self, key) != NULL;
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcHashmap_t *impl_lfcHashmap__IIterable__filter(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retInt__param1_voidPtr fn
) {
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
    if (!self) return 0;

    lfcHashmap_t *result = lfcHashmap_ctorWithSize(impl_lfcHashmap__IIterable__count(_self), self->hash_fn);

    impl_lfcHashmap__IIterable__foreach(_self, lambda(void, (void *iter isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)iter;

        if (fn(pair)) {
        public_lfcHashmap_put(result, pair->key, pair->data);
    }
    }));

    if (free_source == lfcifoc_FREE_OLD_COLLECION) {
        delete(self);
    }
    return result;
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcHashmap_t *impl_lfcHashmap__IIterable__filterNot(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retInt__param1_voidPtr fn
) {
    return impl_lfcHashmap__IIterable__filter(_self, free_source, lambda(int, (void *iter isAnIncognito_param) { return !fn(iter); }));
}

/**
 * Gibt eine neue gefilterte Liste zurueck.
 */
static lfcHashmap_keyValuePair_t *impl_lfcHashmap__IIterable__find(
    lfcIIterable_t *_self,
    lambda_fn__retInt__param1_voidPtr fn
) {
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
    if (!self) return 0;

    size_t i;
    for (i = 0; i < self->table_size; i++) {
        lfcHashmap_element_t *item = &self->table[i];
        lfcHashmap_keyValuePair_t *pair;

        if (item->in_use == 0) {
            continue;
        }

        pair = (lfcHashmap_keyValuePair_t *)item;
        if (fn(pair)) {
            return pair;
        }
    }

    return NULL;
}

/**
 * Iteriert über die liste
 */
static void impl_lfcHashmap__IIterable__foreach(
    lfcIIterable_t *_self,
    lambda_fn__noRet__param1_voidPtr fn
) {
    uint32_t i;
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
    if (!self) return;

    for (i = 0; i < self->table_size; i++) {
        lfcHashmap_element_t *item = &self->table[i];
        lfcHashmap_keyValuePair_t pair;

        if (item->in_use == 0) {
            continue;
        }

        pair.key = item->key;
        pair.data = item->data;
        fn(&pair);
    }
}

/**
 * Gibt 1 zurueck, wenn die Liste leer ist
 *
 * @param _self die Instanz der Liste selbst
 */
static int impl_lfcHashmap__IIterable__isEmpty(
    lfcIIterable_t *_self
) {
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
    if (!self) return 0;

    return self->size == 0;
}

/**
 * Gibt das erste Element der Liste zurueck.
 */
static void *impl_lfcHashmap__IIterable__head(
    lfcIIterable_t *_self __attribute__((unused))
) {
    return NULL;
}

/**
 * Gibt das erste Element der Liste zurueck.
 */
static void *impl_lfcHashmap__IIterable__last(
    lfcIIterable_t *_self __attribute__((unused))
) {
    return NULL;
}

/**
 * Gibt 1 zurueck, wenn die Liste nicht leer ist
 *
 * @param _self die Instanz der Liste selbst
 */
static int impl_lfcHashmap__IIterable__nonEmpty(
    lfcIIterable_t *_self
) {
    return !impl_lfcHashmap__IIterable__isEmpty(_self);
}

/**
 * Erstellt eine neue Map auf Basis der iterator_fn
 */
static lfcHashmap_t *impl_lfcHashmap__IIterable__map(
    lfcIIterable_t *_self,
    lfcIIterable_freeOldCollection_e free_source,
    lambda_fn__retVoidPtr__param1_voidPtr fn
) {
    lfcHashmap_t *self = asInstanceOf(lfcHashmap(), _self);
    if (!self) return 0;

    lfcHashmap_t *result = lfcHashmap_ctorWithSize(impl_lfcHashmap__IIterable__count(_self), self->hash_fn);

    impl_lfcHashmap__IIterable__foreach(_self, lambda(void, (void *iter isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)iter;

        fn(pair);

        public_lfcHashmap_put(result, pair->key, pair->data);
    }));

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
CLASS_CTOR__START(lfcHashmap)
        OVERRIDE_METHOD(lfcHashmap, clear)
        OVERRIDE_METHOD(lfcHashmap, get)
        OVERRIDE_METHOD(lfcHashmap, put)
        OVERRIDE_METHOD(lfcHashmap, remove)
        OVERRIDE_METHOD(lfcHashmap, values)
    CLASS_CTOR__INIT_SUPER(lfcHashmap, lfcObject)
    CLASS_CTOR__INIT_IFACES()
        CLASS_IMPLEMENT_IFACE(lfcHashmap, lfcIIterable)
CLASS_CTOR__END()

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
const lfcHashmap_class_t *lfcHashmap_class(void) {
    return _lfcHashmap_class
           ? _lfcHashmap_class
           : (_lfcHashmap_class = (lfcHashmap_class_t *) new(
            lfcObject_class(), "lfcHashmap_class", lfcObject_class(), sizeof(lfcHashmap_class_t),

            lfcObject_ctor, "", impl_lfcHashmap_class_ctor,

            (void *) 0)
           );
}

const lfcHashmap_t *lfcHashmap(void) {
    return _lfcHashmap
           ? _lfcHashmap
           : (_lfcHashmap = (lfcHashmap_t *) new(
            lfcHashmap_class(),
            "lfcHashmap", lfcObject(), sizeof(lfcHashmap_t),

            lfcObject_ctor, "ctor", public_lfcHashmap_ctor,
            lfcObject_dtor, "dtor", public_lfcHashmap_dtor,
            lfcObject_puto, "puto", public_lfcHashmap_puto,

            lfcHashmap_clear, "clear", public_lfcHashmap_clear,
            lfcHashmap_get, "get", public_lfcHashmap_get,
            lfcHashmap_put, "put", public_lfcHashmap_put,
            lfcHashmap_remove, "remove", public_lfcHashmap_remove,

            lfcHashmap_values, "values", public_lfcHashmap_values,

            lfcIIterable_count, "count", impl_lfcHashmap__IIterable__count,
            lfcIIterable_exists, "exists", impl_lfcHashmap__IIterable__exists,
            lfcIIterable_filter, "filter", impl_lfcHashmap__IIterable__filter,
            lfcIIterable_filterNot, "filterNot", impl_lfcHashmap__IIterable__filterNot,
            lfcIIterable_find, "find", impl_lfcHashmap__IIterable__find,
            lfcIIterable_foreach, "foreach", impl_lfcHashmap__IIterable__foreach,
            lfcIIterable_isEmpty, "isEmpty", impl_lfcHashmap__IIterable__isEmpty,
            lfcIIterable_head, "head", impl_lfcHashmap__IIterable__head,
            lfcIIterable_last, "last", impl_lfcHashmap__IIterable__last,
            lfcIIterable_map, "map", impl_lfcHashmap__IIterable__map,
            lfcIIterable_nonEmpty, "nonEmpty", impl_lfcHashmap__IIterable__nonEmpty,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcHashmap)

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Map mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcHashmap_t *lfcHashmap_ctor(
    lfcHashmap_hash_fn hash_fn
) {
    return lfcHashmap_ctorWithSize(DEFAULT_HASHMAP_INITIAL_SIZE, hash_fn);
}

/**
 * Map mit den angegebenen Größen initialisieren
 *
 * @param initial_size ist die initiale Größe der Liste
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcHashmap_t *lfcHashmap_ctorWithSize(
    size_t initial_size,
    lfcHashmap_hash_fn hash_fn
) {
    return (lfcHashmap_t *)new(lfcHashmap(), initial_size, hash_fn);
}

lfcOOP_accessor(lfcHashmap, clear, int)
lfcOOP_accessor(lfcHashmap, get, void *, const void *)
lfcOOP_accessor(lfcHashmap, put, int, const void *, void *)
lfcOOP_accessor(lfcHashmap, remove, int, const void *)
lfcOOP_accessor(lfcHashmap, values, lfcList_t *)
