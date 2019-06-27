#ifndef LIBFORC_COLLECTION_LFCLIST_H
#define LIBFORC_COLLECTION_LFCLIST_H

  #include <stdint.h>
  #include <pthread.h>
  #include "../core/lfcObject.h"
  #include "lfcIIterable.h"

  #define DEFAULT_LIST_INITIAL_SIZE         10
  #define DEFAULT_LIST_INCREASE_BY          10

  DEFINE_CLASS(lfcList)

  typedef int (*lfcList_compare_fn)(void *a, void *b);

  struct lfcList { const struct lfcObject _;
    void **table;
    size_t table_size;
    size_t size;
    size_t increaseBy;

    pthread_mutex_t lock;
  };

  struct lfcList_class { const struct lfcObject_class _;
    method_t add;
    method_t addList;
    method_t clear;
    method_t getAt;
    method_t indexOf;
    method_t insertAt;
    method_t mergeList;
    method_t remove;
    method_t removeAt;

    method_t bubbleSort;

    lfcIIterable_t lfcIIterable;
  };

  struct lfcList_methods {
    int     (*add      )(lfcList_t *_self, void *toAdd);
    int     (*addList  )(lfcList_t *_self, lfcList_t *toAdd);
    int     (*clear    )(lfcList_t *_self);
    void   *(*getAt    )(lfcList_t *_self, size_t idx);
    size_t  (*indexOf  )(lfcList_t *_self, const void *);
    int     (*insertAt )(lfcList_t *_self, size_t idx, void *toAdd);
    int     (*mergeList)(lfcList_t *_self, lfcList_t *toAdd);
    int     (*remove   )(lfcList_t *_self, const void *toRemove);
    int     (*removeAt )(lfcList_t *_self, size_t idx);

    int     (*bubbleSort)(lfcList_t *_self, lfcList_compare_fn compare_fn);

    // super
    const lfcObject_methods_t *base;

    // ifaces
    const lfcIIterable_methods_t *lfcIIterable;
  };

  lfcList_t *lfcList_ctor(void);
  lfcList_t *lfcList_ctorWithSize(size_t initial_size, size_t increase_by);

  int     lfcList_add      (lfcList_t *_self, void *toAdd);
  int     lfcList_addList  (lfcList_t *_self, lfcList_t *toAdd);
  int     lfcList_clear    (lfcList_t *_self);
  void   *lfcList_getAt    (lfcList_t *_self, size_t idx);
  size_t  lfcList_indexOf  (lfcList_t *_self, const void *);
  int     lfcList_insertAt (lfcList_t *_self, size_t idx, void *toAdd);
  int     lfcList_mergeList(lfcList_t *_self, lfcList_t *toAdd);
  int     lfcList_remove   (lfcList_t *_self, const void *toRemove);
  int     lfcList_removeAt (lfcList_t *_self, size_t idx);

  int     lfcList_bubbleSort(lfcList_t *self, lfcList_compare_fn compare_fn);

#endif //LIBFORC_COLLECTION_LFCLIST_H
