#include "lfcTuple.h"

#include <stdlib.h>

lfcTuple2_t *lfcTuple2_create(
  void *_1,
  void *_2
) {
  lfcTuple2_t *ret = calloc(1, sizeof(lfcTuple2_t));

  if (ret) {
    ret->_1 = _1;
    ret->_2 = _2;
  }

  return ret;
}

lfcTuple3_t *lfcTuple3_create(
  void *_1,
  void *_2,
  void *_3
) {
  lfcTuple3_t *ret = calloc(1, sizeof(lfcTuple3_t));

  if (ret) {
    ret->_1 = _1;
    ret->_2 = _2;
    ret->_3 = _3;
  }

  return ret;
}

lfcTuple4_t *lfcTuple4_create(
  void *_1,
  void *_2,
  void *_3,
  void *_4
) {
  lfcTuple4_t *ret = calloc(1, sizeof(lfcTuple4_t));

  if (ret) {
    ret->_1 = _1;
    ret->_2 = _2;
    ret->_3 = _3;
    ret->_4 = _4;
  }

  return ret;
}

lfcTuple5_t *lfcTuple5_create(
  void *_1,
  void *_2,
  void *_3,
  void *_4,
  void *_5
) {
  lfcTuple5_t *ret = calloc(1, sizeof(lfcTuple5_t));

  if (ret) {
    ret->_1 = _1;
    ret->_2 = _2;
    ret->_3 = _3;
    ret->_4 = _4;
    ret->_5 = _5;
  }

  return ret;
}
