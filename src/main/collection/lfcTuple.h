#ifndef LIBFORC_COLLECTION_TUPLE_H
#define LIBFORC_COLLECTION_TUPLE_H

  typedef struct lfcTuple2 lfcTuple2_t;
  struct lfcTuple2 {
      void *_1;
      void *_2;
  };

  typedef struct lfcTuple3 lfcTuple3_t;
  struct lfcTuple3 {
      void *_1;
      void *_2;
      void *_3;
  };

  typedef struct lfcTuple4 lfcTuple4_t;
  struct lfcTuple4 {
      void *_1;
      void *_2;
      void *_3;
      void *_4;
  };

  typedef struct lfcTuple5 lfcTuple5_t;
  struct lfcTuple5 {
      void *_1;
      void *_2;
      void *_3;
      void *_4;
      void *_5;
  };

  lfcTuple2_t *lfcTuple2_create(void *_1, void *_2);
  lfcTuple3_t *lfcTuple3_create(void *_1, void *_2, void *_3);
  lfcTuple4_t *lfcTuple4_create(void *_1, void *_2, void *_3, void *_4);
  lfcTuple5_t *lfcTuple5_create(void *_1, void *_2, void *_3, void *_4, void *_5);


#endif //LIBFORC_COLLECTION_TUPLE_H
