#ifndef LIBFORC_TESTING_CORE_MOCKS_CLASSA_H
#define LIBFORC_TESTING_CORE_MOCKS_CLASSA_H

  #include "core/lfcObject.h"

typedef struct ClassA ClassA_t;
  typedef struct ClassAClass ClassAClass_t;

  extern const ClassA_t *ClassA (void);
  extern const ClassAClass_t * ClassAClass (void);

  struct ClassA { const struct lfcObject _;
    int x;
    char *txt;
  };

  struct ClassAClass { const struct lfcObject_class _;
    method_t print;
  };

  void classA_print(const ClassA_t *_self);

#endif //LIBFORC_TESTING_CORE_MOCKS_CLASSA_H
