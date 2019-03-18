#ifndef LIBFORC_COLLECTION_LFCHASHMAP_H
#define LIBFORC_COLLECTION_LFCHASHMAP_H

  #include <stdint.h>
  #include <pthread.h>
  #include "../core/lfcObject.h"
  #include "lfcIIterable.h"
  #include "lfcList.h"

  #define DEFAULT_HASHMAP_INITIAL_SIZE         10

  typedef struct lfcHashmap               lfcHashmap_t;
  typedef struct lfcHashmap_class         lfcHashmap_class_t;
  typedef struct lfcHashmap_methods       lfcHashmap_methods_t;
  typedef struct lfcHashmap_element       lfcHashmap_element_t;
  typedef struct lfcHashmap_keyValuePair  lfcHashmap_keyValuePair_t;

  /******************************************************************************************/
  /* HASH METHODS                                                                           */
  /******************************************************************************************/
  typedef unsigned int (*lfcHashmap_hash_fn)(lfcHashmap_t *, const void *);
  unsigned int lfcHashmap_hash_int     (lfcHashmap_t *self, const void *key);
  unsigned int lfcHashmap_hash_ptr     (lfcHashmap_t *self, const void *key);
  unsigned int lfcHashmap_hash_string  (lfcHashmap_t *self, const void *key);

  struct lfcHashmap_element {
      const void *key;
      void *data;

      unsigned char in_use;
  };

  struct lfcHashmap_keyValuePair {
      const void *key;
      void *data;
  };

  struct lfcHashmap { const struct lfcObject _;
      lfcHashmap_element_t *table;
      size_t table_size;
      size_t size;
      lfcHashmap_hash_fn hash_fn;

      pthread_mutex_t lock;
  };

  struct lfcHashmap_class { const struct lfcObject_class _;
      method_t clear;
      method_t get;
      method_t put;
      method_t remove;

      method_t values;

      lfcIIterable_t lfcIIterable;
  };

  struct lfcHashmap_methods {
    int   (*clear )(lfcHashmap_t *_self);
    void *(*get   )(lfcHashmap_t *_self, const void *key);
    int   (*put   )(lfcHashmap_t *_self, const void *key, void *value);
    int   (*remove)(lfcHashmap_t *_self, const void *key);

    lfcList_t *(*values)(lfcHashmap_t *_self);

    // super
    const lfcObject_methods_t *base;

    // ifaces
    const lfcIIterable_methods_t *lfcIIterable;
  };

  extern const lfcHashmap_t       *lfcHashmap(void);
  extern const lfcHashmap_class_t *lfcHashmap_class(void);
  extern const lfcHashmap_methods_t *lfcHashmap_methods(void);

  #define LFCHASH__CREATE_INTKEY_HASHMAP()      lfcHashmap_ctor(lfcHashmap_hash_int)
  #define LFCHASH__CREATE_PTRKEY_HASHMAP()      lfcHashmap_ctor(lfcHashmap_hash_ptr)
  #define LFCHASH__CREATE_STRINGKEY_HASHMAP()   lfcHashmap_ctor(lfcHashmap_hash_string)

  lfcHashmap_t *lfcHashmap_ctor(lfcHashmap_hash_fn hash_fn);
  lfcHashmap_t *lfcHashmap_ctorWithSize(size_t initial_size, lfcHashmap_hash_fn hash_fn);

  int   lfcHashmap_clear (lfcHashmap_t *_self);
  void *lfcHashmap_get   (lfcHashmap_t *_self, const void *key);
  int   lfcHashmap_put   (lfcHashmap_t *_self, const void *key, void *value);
  int   lfcHashmap_remove(lfcHashmap_t *_self, const void *key);
  lfcList_t *lfcHashmap_values(lfcHashmap_t *_self);

#endif //LIBFORC_COLLECTION_LFCHASHMAP_H
