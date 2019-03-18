#ifndef LIBFORC_COLLECTION_LFCBITSET_H
#define LIBFORC_COLLECTION_LFCBITSET_H

  #include <stdint.h>
  #include <pthread.h>
  #include "../core/lfcObject.h"
  #include "lfcIIterable.h"

  DEFINE_CLASS(lfcBitset)

  struct lfcBitset { const struct lfcObject _;
      char *array;
      size_t array_count;

      pthread_mutex_t lock;
  };

  struct lfcBitset_class { const struct lfcObject_class _;
      method_t count;

      method_t clear;
      method_t get;
      method_t set;

      method_t toggle;
  };

  struct lfcBitset_methods {
      int    (*clear     )(lfcBitset_t *_self, size_t index);
      size_t (*count     )(lfcBitset_t *_self);
      int    (*get       )(lfcBitset_t *_self, size_t index);
      int    (*set       )(lfcBitset_t *_self, size_t index);
      int    (*toggle    )(lfcBitset_t *_self, size_t index);

      // super
      const lfcObject_methods_t *base;
  };

  lfcBitset_t *lfcBitset_ctor(void);
  lfcBitset_t *lfcBitset_ctor_withBits(size_t argc, ...);

  int    lfcBitset_clear (lfcBitset_t *_self, size_t index);
  size_t lfcBitset_count(lfcBitset_t *_self);
  int    lfcBitset_get   (lfcBitset_t *_self, size_t index);
  int    lfcBitset_set   (lfcBitset_t *_self, size_t index);
  int    lfcBitset_toggle(lfcBitset_t *_self, size_t index);

#endif //LIBFORC_COLLECTION_LFCBITSET_H
