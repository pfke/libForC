#include "lfcObject.h"

#include <signal.h>
#include <stdlib.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcObject_class_t _Object;
static const lfcObject_class_t _Class;
static lfcObject_methods_t _lfcObject_methods;

/***** Prototypes *****/
      struct lfcObject *allocate(const void *_self);
const lfcObject_class_t *super   (const void *_self);

/***** Object *****/

/**
 * Instantiate a new object.
 *
 * @param _self is the object type to create
 * @param app are the params for the ctor
 *
 * @return a new instantiated and ctored object
 */
static struct lfcObject *impl_object_new (
  const void *_self,
  va_list *app
) {
  return lfcObject_ctor(
    allocate(asInstanceOf(lfcObject_class(), _self)),
    app
  );
}

/**
 * This is the ctor for this base object.
 *
 * @param _self is the object type to create
 * @param app are the params
 *
 * @return the passed object
 */
static void *impl_object_ctor (
  void *_self,
  va_list *app __attribute__((unused))
) {
  return asInstanceOf(lfcObject(), _self);
}

/**
 * This is the dtor for this base object.
 *
 * @param _self is the object type to create
 *
 * @return the passed object
 */
static void *impl_object_dtor (
  void *_self
) {
  return asInstanceOf(lfcObject(), _self);
}

/**
 * Print some basic oop infos.
 *
 * @param _self is the object itself
 *
 * @return the fprintf return value
 */
static int impl_object_puto (
  const void *_self,
  FILE *fp
) {
  const lfcObject_class_t * class;
  struct lfcObject * self = asInstanceOf(lfcObject(), _self);

  if (self == NULL) return -1;

  class = getClass(self);
  if (class == NULL) return -1;
  return fprintf(fp, "%s at %p\n", class->name, self);
}

/**
 * This method calls the dtor and deletes the object.
 *
 * @param _self is the object itself
 */
static void impl_object_delete (
  void *_self
) {
    lfcObject_t *self = asInstanceOf(lfcObject(), _self);
    lfcObject_class_t *clazz = asInstanceOf(lfcObject_class(), self->class);

    lfcObject_dtor(self);
    memset(_self, 0, clazz->instance_size);

    free(_self);
}

/**
 * This method forwards the given params to the named function.
 * This is the last block in the chain, so there will be an assert, if the calling graph will end here at one time.
 *
 * @param _self is the object itself
 * @param result store that result in this prt
 * @param selector
 * @param name is the name of the function to forward to
 * @param app are the params to lfcObject_forward
 */
static void impl_object_forward (
  const void *_self,
  void *result __attribute__((unused)),
	method_fn selector __attribute__((unused)),
  const char *name,
  va_list *app __attribute__((unused))
) {
  const struct lfcObject * self = asInstanceOf(lfcObject(), _self);

  fprintf(stderr, "%s at %p does not answer %s\n", nameOf(getClass(self)), self, name);
#ifdef AHA_DEBUG
  abort();
#endif // #ifdef AHA_DEBUG
}

/**
 * Return the class from the given object.
 */
const lfcObject_class_t *getClass(
  const void *_self
) {
  const struct lfcObject * self = asInstanceOf(lfcObject(), _self);

  if (!self) return NULL;

  return self->class;
}

/**
 * Return instance size of the given class/object.
 */
size_t sizeOf (
  const void * _self
) {
  const lfcObject_class_t * class = getClass(_self);

  if (!class) return 0;

  return class->instance_size;
}

/**
 * Returns 1, if the given object is from the given class.
 */
int isInstanceOf(
  const void *class,
  const void *_self
) {
  if (_self) {
    const lfcObject_class_t * myClass;
    const struct lfcObject * self = asInstanceOf(lfcObject(), _self);

    if (NULL == asInstanceOf(lfcObject_class(), class)) {
      return 0;
    }

    myClass = getClass(self);
    if (class != lfcObject()) {
      while (myClass != class) {
        if (NULL == myClass) { return 0; }
        if (myClass == lfcObject()) { return 0; }

        myClass = super(myClass);
      }
    }
    return 1;
  }
  return 0;
}

/**
 * Safely cast to this instance.
 */
void *
asInstanceOf(
  const void *class,
  const void *_self
) {
  const struct lfcObject *self;
  const lfcObject_class_t *myClass;

  if (!class || ((struct lfcObject *)class)->magic != MAGIC) return NULL;
  if (!_self || ((struct lfcObject *)_self)->magic != MAGIC) return NULL;

  self = (const struct lfcObject *)_self;
  myClass = (const lfcObject_class_t *)self->class;

  if (class != lfcObject()) {
    while (myClass != class) {
      if (!myClass || ((struct lfcObject *)myClass)->magic != MAGIC) return NULL;
      if (myClass == lfcObject()) return NULL;

      myClass = myClass->super;
    }
  }

  return (void *) self;
}

const method_t *respondsTo(
  const void *_self,
  const char *tag
) {
  if (tag && * tag) {
    const lfcObject_class_t *class;
    const method_t *p;

    class = getClass(_self);
    if (!class) return NULL;

    p = &class->ctor;
    if (!p) return NULL;

    int nmeth = (sizeOf(class) - offsetof(lfcObject_class_t, ctor)) / sizeof(method_t);

    do {
      if (p->tag && strcmp(p->tag, tag) == 0) {
        return p->method ? p : 0;
      }
    } while (++p, --nmeth);
  }
  return 0;
}

const char *nameOf (
  const void *_self
) {
  const lfcObject_class_t * self = asInstanceOf(lfcObject_class(), _self);

  return (self) ? self->name : "UNKNOWN";
}

struct lfcObject * allocate (
  const void *_self
) {
  struct lfcObject * object;
  const lfcObject_class_t * self = asInstanceOf(lfcObject_class(), _self);

  if (!self) return NULL;
  if (!self->instance_size) return NULL;

  object = calloc(1, self->instance_size);
  if (!object) return NULL;

  object->magic = MAGIC;
  object->class = self;
  return object;
}

const lfcObject_class_t * super (const void * _self) {
  const lfcObject_class_t * self = asInstanceOf(lfcObject_class(), _self);

  return (self == NULL) ? NULL : self->super;
}

IMPL_API__wRET__w_1PARAM(lfcObject, ctor, void *, va_list *)
IMPL_API__wRET__w_0PARAM(lfcObject, dtor, void *)
IMPL_API__wRET__w_1PARAM(lfcObject, puto, int, FILE *)

void lfcObject_forward(
  const void *_self,
  void *result,
  method_fn selector,
  const char *name,
  ...
) {
  va_list ap;
  const lfcObject_class_t * class = getClass(_self);

  if (class == NULL) return;

  va_start(ap, name);
  if (class->forward.method) {
    ((void (*) (const void *, void *, method_fn, const char *, va_list *)) class->forward.method)(_self, result, selector, name, & ap);
  } else {
    //assert(0);
  }
  va_end(ap);
}

void delete (
  void *_self
) {
  const lfcObject_class_t * class = getClass(_self);

  if (class == NULL) {
//      fprintf(stderr, "%p is not a class\n", _self);
      return;
  }

  if (class->delete.method) {
    ((void (*) (void *))class->delete.method)(_self);
  } else {
    lfcObject_forward(_self, 0, (method_fn) delete, "delete", _self);
  }
}

struct lfcObject *new (
  const void *_self,
  ...
) {
  struct lfcObject * result;
  va_list ap;
  const lfcObject_class_t * class = asInstanceOf(lfcObject_class(), _self);

  if (class == NULL) return NULL;

  va_start(ap, _self);
  if (class->new.method) {
    result = ((struct lfcObject * (*) (const void *, va_list *))class->new.method)(_self, & ap);
  } else
    lfcObject_forward(_self, &result, (method_fn) new, "new", _self, &ap);
  va_end(ap);
  return result;
}

/**
 * Call the ctor of the given object super class.
 * Can be used if its not wanted to pass the same args as in the calling ctor.
 *
 * @param _class is the class descriptor
 * @param _self is the newly created object
 * @param ... are ctor params
 *
 * @return the class itself
 */
void *lfcObject_super_ctor(
  const void *_class,
  void *_self,
  ...
) {
  va_list ap;
  const lfcObject_class_t *superclass = super(_class);

  if (superclass == NULL) return NULL;
  if (!superclass->ctor.method) return NULL;

  va_start(ap, _self);
  return ((void *(*)(const void *, va_list *))superclass->ctor.method)(_self, &ap);
  va_end(ap);
}

/**
 * Call the ctor of the given object super class
 *
 * @param _class is the class descriptor
 * @param _self is the newly created object
 * @param app are the ctor params
 *
 * @return
 */
SUPER__RET__w_1PARAM(lfcObject, vsuper_ctor, ctor, void *, va_list *)
SUPER__RET__w_0PARAM(lfcObject, super_delete, delete, void)
SUPER__RET__w_0PARAM(lfcObject, super_dtor, dtor, void *)
SUPER__RET__w_1PARAM(lfcObject, super_puto, puto, int, FILE *)
SUPER__RET__w_1PARAM(lfcObject, super_new, new, struct lfcObject *, va_list *)
SUPER__RET__w_4PARAM(lfcObject, super_forward, forward, void, void *, method_fn, const char *, va_list *)

/***** Class *****/
static void *
impl_class_ctor (
  void *_self,
  va_list *app
) {
  lfcObject_class_t *self = _self;
  const size_t offset = offsetof(lfcObject_class_t, ctor);
  method_fn selector;
  va_list ap = {0};

  self->name = va_arg(*app, char*);
  self->super = asInstanceOf(lfcObject_class(), va_arg(*app, void*));
  self->instance_size = va_arg(*app, size_t);

  memcpy((char*)self + offset, (char*)self->super + offset, sizeOf(self->super) - offset);

#ifdef va_copy
  va_copy(ap, *app);
#else
  ap = *app;
#endif

  while ((selector = va_arg(ap, method_fn))) {
    const char * tag = va_arg(ap, const char *);
    method_fn method = va_arg(ap, method_fn);

    OVERRIDE_METHOD_NOT_ACCESSIBLE(lfcObject, ctor)
    OVERRIDE_METHOD_NOT_ACCESSIBLE(lfcObject, dtor)
    OVERRIDE_METHOD_NOT_ACCESSIBLE(lfcObject, forward)
    OVERRIDE_METHOD(lfcObject, puto)

    if (selector == (method_fn)delete) {
      if (tag) {
        self->delete.tag = tag;
        self->delete.selector = selector;
      }
      self -> delete.method = method;
      continue;
    }
    if (selector == (method_fn)new) {
      if (tag) {
        self->new.tag = tag;
        self->new.selector = selector;
      }
      self -> new.method = method;
      continue;
    }
  }

#ifdef va_copy
  va_end(ap);
#endif

  return self;
}

static void *
impl_class_dtor(
  void *_self __attribute__((unused))
) {
  //assert(0);
  return 0;
}

static void
impl_class_delete(
  void *_self
) {
  lfcObject_class_t *self = asInstanceOf(lfcObject_class(), _self);

  if (self == NULL) return;

  fprintf(stderr, "%s: cannot delete class\n", self->name);
}

/***** Initialization *****/
const void *lfcObject(void) { return & _Object; }
const void *lfcObject_class(void) { return & _Class; }
const lfcObject_methods_t *lfcObject_methods() { return &_lfcObject_methods; }

static const lfcObject_class_t _Object = {
  { MAGIC, &_Class }, "Object", &_Object, sizeof(lfcObject_t),

  { "",     (method_fn) 0,                 (method_fn) impl_object_ctor},
  { "",     (method_fn) 0,                 (method_fn) impl_object_dtor},
  { "puto", (method_fn) lfcObject_puto,    (method_fn) impl_object_puto},
  { "",     (method_fn) delete,            (method_fn) impl_object_delete},
  { "",     (method_fn) lfcObject_forward, (method_fn) impl_object_forward},
  { "",     (method_fn) 0,                 (method_fn) impl_object_new},
};

static const lfcObject_class_t _Class = {
  { MAGIC, &_Class }, "Class", &_Object, sizeof(lfcObject_class_t),

  { "",        (method_fn) 0,                 (method_fn) impl_class_ctor},
  { "",        (method_fn) 0,                 (method_fn) impl_class_dtor},
  { "puto",    (method_fn) lfcObject_puto,    (method_fn) impl_object_puto},
  { "delete",  (method_fn) delete,            (method_fn) impl_class_delete},
  { "forward", (method_fn) lfcObject_forward, (method_fn) impl_object_forward},
  { "",        (method_fn) 0,                 (method_fn) impl_object_new},
};
