#ifndef LIBFORC_COLLECTION_LFCIITERABLE_H
#define LIBFORC_COLLECTION_LFCIITERABLE_H

  #include "../core/lfcCore.h"
  #include "../core/lfcObject.h"

  typedef enum lfcIIterable_freeOldCollection {
      lfcifoc_FREE_OLD_COLLECION,
      lfcifoc_DONT_FREE_OLD_COLLECION,
  }lfcIIterable_freeOldCollection_e;

  typedef struct lfcIIterable_methods lfcIIterable_methods_t;

  typedef struct lfcIIterable {
      method_t count;
      method_t exists;
      method_t filter;
      method_t filterNot;
      method_t find;
      method_t foreach;
      method_t isEmpty;
      method_t head;
      method_t last;
      method_t map;
      method_t next;
      method_t nonEmpty;
  } lfcIIterable_t;

  struct lfcIIterable_methods {
      size_t (*count    )(void *_self);
      int    (*exists   )(void *_self, const void *toCheck);
      void  *(*filter   )(void *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
      void  *(*filterNot)(void *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr fn);
      void  *(*find     )(void *_self, lambda_fn__retInt__param1_voidPtr fn);
      void   (*foreach  )(void *_self, lambda_fn__noRet__param1_voidPtr fn);
      int    (*isEmpty  )(void *_self);
      void  *(*head     )(void *_self);
      void  *(*last     )(void *_self);
      void  *(*map      )(void *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retVoidPtr__param1_voidPtr fn);
      void  *(*next     )(void *_self, const void *current);
      int    (*nonEmpty )(void *_self);
  };

  lfcIIterable_t * lfcIIterable_init(lfcIIterable_t *iface, va_list *app);
  extern const lfcIIterable_methods_t *lfcIIterable_methods(void);

  /**
   * Returns the list size.
   */
  size_t lfcIIterable_count(void *_self);

  /**
   * Returns 1 if the given element exists in the iterable list.
   */
  int lfcIIterable_exists(void *_self, const void *toCheck);

  /**
   * Gibt eine neue gefilterte Liste zurueck (filter_method muss 1 zurueckgeben um in die Liste aufgenommen zu werden).
   */
  void *lfcIIterable_filter(void *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr);

  /**
   * Gibt eine neue gefilterte Liste zurueck (filter_method muss 0 zurueckgeben um in die Liste aufgenommen zu werden).
   */
  void *lfcIIterable_filterNot(void *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retInt__param1_voidPtr);

  /**
   * Sucht ein Element (lambda mus 1 zurueckgeben) und gibt das erste gefunden Element in der Auflistung zurueck.
   */
  void *lfcIIterable_find(void *_self, lambda_fn__retInt__param1_voidPtr);

  /**
   * Iteriert über eine Liste.
   */
  void lfcIIterable_foreach(void *_self, lambda_fn__noRet__param1_voidPtr);

  /**
   * Returns 1 if empty and 0 of not.
   */
  int lfcIIterable_isEmpty(void *_self);

  /**
   * Returns the first element of the list.
   */
  void *lfcIIterable_head(void *_self);

  /**
   * Returns the last element of the list.
   */
  void *lfcIIterable_last(void *_self);

  /**
   * Monadic op -> build a new collection
   */
  void *lfcIIterable_map(void *_self, lfcIIterable_freeOldCollection_e free_source, lambda_fn__retVoidPtr__param1_voidPtr);

  /**
   * Returns the next element of the list.
   */
  void *lfcIIterable_next(void *_self, const void *current);

  /**
   * Returns !lfcIIterable_isEmpty(...)
   */
  int lfcIIterable_nonEmpty(void *_self);

#endif //LIBFORC_COLLECTION_LFCIITERABLE_H
