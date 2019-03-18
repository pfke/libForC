#ifndef LIBFORC_COLLECTION_LFCQUEUE_H
#define LIBFORC_COLLECTION_LFCQUEUE_H

  #include <stdint.h>
  #include <pthread.h>
  #include "../core/lfcObject.h"
  #include "lfcIIterable.h"
  #include "lfcList.h"

  #define DEFAULT_QUEUE_INITIAL_SIZE         10
  #define DEFAULT_QUEUE_INCREASE_BY          10

  DEFINE_CLASS(lfcQueue)

  struct lfcQueue { const struct lfcObject _;
      lfcList_t *table;

      pthread_mutex_t lock;
  };

  struct lfcQueue_class { const struct lfcObject_class _;
      method_t clear;
      method_t enqueue;
      method_t dequeue;

      lfcIIterable_t lfcIIterable;
  };

  struct lfcQueue_methods {
      int     (*clear  )(lfcQueue_t *_self);
      int     (*enqueue)(lfcQueue_t *_self, void *toAdd);
      void   *(*dequeue)(lfcQueue_t *_self);

      // super
      const lfcObject_methods_t *base;

      // ifaces
      const lfcIIterable_methods_t *lfcIIterable;
  };

  lfcQueue_t *lfcQueue_ctor(void);
  lfcQueue_t *lfcQueue_ctorWithSize(size_t initial_size, size_t increase_by);

  int     lfcQueue_clear  (lfcQueue_t *_self);
  int     lfcQueue_enqueue(lfcQueue_t *_self, void *toAdd);
  void   *lfcQueue_dequeue(lfcQueue_t *_self);

#endif //LIBFORC_COLLECTION_LFCQUEUE_H
