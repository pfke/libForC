#include "ClassA.h"

#include <stdarg.h>
#include <stdio.h>

static void *impl_classA_ctor(
  void *_self,
  va_list *app
) {
  ClassA_t *self = (ClassA_t *) lfcObject_super_ctor(ClassA(), _self);

  self->x = va_arg(* app, int);
  self->txt = va_arg(* app, char *);

  return self;
}

static void impl_classA_print(
  const void *_self
) {
  const struct ClassA * self = asInstanceOf(ClassA(), _self);

  printf("x = %d, txt = '%s'\n", self->x, self->txt);
}

static void *impl_classAClass_ctor(
  void *_self,
  va_list *app
) {
  struct ClassAClass * self = lfcObject_vsuper_ctor(ClassAClass(), _self, app);
  method_fn selector;
#ifdef va_copy
  va_list ap = {0};
  va_copy(ap, * app);
#else
  va_list ap = * app;
#endif

  while ((selector = va_arg(ap, method_fn)))
  {	const char * tag = va_arg(ap, const char *);
    method_fn method = va_arg(ap, method_fn);

    if (selector == (method_fn) classA_print)
    {	if (tag)
        self -> print.tag = tag,
          self -> print.selector = selector;
      self -> print.method = method;
      continue;
    }
  }
#ifdef va_copy
  va_end(ap);
#endif
  return self;
}

void classA_print(
  const ClassA_t *_self
) {
  const ClassAClass_t *class = (const ClassAClass_t *) getClass(_self);

  if (isInstanceOf(ClassAClass(), class) && class->print.method) {

    ((void (*) (const void *)) class -> print.method)(_self);
  } else
    lfcObject_forward(_self, 0, (method_fn) classA_print, "print", _self);
}

static const ClassAClass_t *_ClassAClass;
static const ClassA_t *_ClassA;

const ClassAClass_t *ClassAClass (void) {
  return _ClassAClass
    ? _ClassAClass
    : (_ClassAClass = (ClassAClass_t*) new(
      lfcObject_class(), "ClassAClass", lfcObject_class(), sizeof(struct ClassAClass),
      lfcObject_ctor, "", impl_classAClass_ctor,
      (void *) 0)
    );
}

const ClassA_t *ClassA (void) {
  return _ClassA
    ? _ClassA
    : (_ClassA = (ClassA_t*) new(
      ClassAClass(),
      "ClassA", lfcObject(), sizeof(struct ClassA),
      lfcObject_ctor, "lfcObject_ctor", impl_classA_ctor,
      classA_print, "print", impl_classA_print,
      (void *) 0)
    );
}
