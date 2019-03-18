#ifndef LIBFORC_CORE_LFCOBJECT_H
#define LIBFORC_CORE_LFCOBJECT_H

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define   MAGIC  0x0effaced
#define   abstract
#define   sealed

typedef void (* method_fn) (void);

typedef struct lfcObject         lfcObject_t;
typedef struct lfcObject_class   lfcObject_class_t;
typedef struct lfcObject_methods lfcObject_methods_t;
typedef struct lfcObject_method  method_t;

struct lfcObject_method {
    const char *tag;
    method_fn selector;
    method_fn method;
};

struct lfcObject {
    unsigned long magic;
    const void *class;
};

extern const void                *lfcObject(void);
extern const void                *lfcObject_class(void);
extern const lfcObject_methods_t *lfcObject_methods(void);

// methods with static linkage
const lfcObject_class_t *getClass    (const void *_self);
size_t                   sizeOf      (const void *_self);
int                      isInstanceOf(const void *class, const void *_self);
void                    *asInstanceOf(const void *class, const void *_self);
const method_t          *respondsTo  (const void *_self, const char *tag);
const char              *nameOf      (const void *_self);

struct lfcObject_class {
    lfcObject_t _;
    const char *name;
    const void *super;
    size_t instance_size;

    method_t ctor;
    method_t dtor;
    method_t puto;
    method_t delete;
    method_t forward;
    method_t new;
};

struct lfcObject_methods {
    int  (*puto)(lfcObject_t *_self, FILE *fp);
};

// dynamic linkage methods
void              *lfcObject_ctor   (      lfcObject_t *_self, va_list *app);
void              *lfcObject_dtor   (      lfcObject_t *_self);
int                lfcObject_puto   (      lfcObject_t *_self, FILE *fp);
void                         delete (      void        *_self);
void               lfcObject_forward(const void        *_self, void *result, method_fn selector, const char *name, ...);
struct lfcObject            *new    (const void        *_self, ...);

/**
   * Call the super ctor and passing a va_list
   */
void              *lfcObject_super_ctor   (const void *_class, void *_self, ...);
void              *lfcObject_vsuper_ctor  (const void *_class, void *_self, va_list *app);
void              *lfcObject_super_dtor   (const void *_class, void *_self);
int                lfcObject_super_puto   (const void *_class, void *_self, FILE *fp);
void               lfcObject_super_delete (const void *_class, void *_self);
void               lfcObject_super_forward(const void *_class, void *_self, void *result, method_fn selector, const char *name, va_list *app);
struct lfcObject  *lfcObject_super_new    (const void *_class, void *_self, va_list *app);

/******************************************************************************************/
/* HELPER MACROS                                                                          */
/******************************************************************************************/
#define STRINGIZE(x)  #x

/**
   * Macro zur Erstellung der Klassendefinition:
   *
   *   .structs fuer Objekt, Klasse + Methoden
   *   .Funktoins-Deklaration fuer Objekt, Klasse + Methoden
   */
#define DEFINE_CLASS(name) \
        typedef struct name             name##_t;           \
        typedef struct name##_class     name##_class_t;     \
        typedef struct name##_methods   name##_methods_t;   \
        extern const name##_t          *name(void);         \
        extern const name##_class_t    *name##_class(void); \
        extern const name##_methods_t  *name##_methods(void);
/**
   * Macro zur Erstellung der Klassen-ctor Initialisierungs-Funktion
   *
   * Aufruf:
   *   CLASS_CTOR__START(lfcList)
   *     OVERRIDE_METHOD(lfcList, add)
   *     OVERRIDE_METHOD(lfcList, clear)
   *   CLASS_CTOR__IFACES()
   *     lfcIIterable_init(&self->iterable, app);
   *   CLASS_CTOR__END()
   *
   * Resultat:
   *   static void *impl_lfcListClass_ctor(
   *     void *_self,
   *     va_list *app
   *   ) {
   *     lfcList_class_t *self = lfcObject_vsuper_ctor(lfcList_class(), _self, app);
   *     method_fn selector;
   *   #ifdef va_copy
   *     va_list ap = {0};
   *     va_copy(ap, *app);
   *   #else
   *     va_list ap = *app;
   *   #endif
   *
   *     while ((selector = va_arg(ap, method_fn))) {
   *       const char * tag = va_arg(ap, const char *);
   *       method_fn method = va_arg(ap, method_fn);
   *
   *       OVERRIDE_METHOD(lfcList, add)
   *       OVERRIDE_METHOD(lfcList, clear)
   *     }
   *
   *     lfcIIterable_init(&self->iterable, app);
   *
   *   #ifdef va_copy
   *     va_end(ap);
   *   #endif
   *     return self;
   *   }
   */
#define CLASS_CTOR__START(PREFIX) \
      static void *impl_##PREFIX##_class_ctor(                                                                          \
        void *_self,                                                                                                    \
        va_list *app                                                                                                    \
      ) {                                                                                                               \
        PREFIX##_class_t *self = lfcObject_vsuper_ctor(PREFIX##_class(), _self, app);                                   \
        method_fn selector;                                                                                             \
        va_list ap = {0};                                                                                               \
        va_copy(ap, *app);                                                                                              \
                                                                                                                        \
        if (!self) return NULL;                                                                                         \
                                                                                                                        \
        while ((selector = va_arg(ap, method_fn))) {                                                                    \
          const char * tag __attribute__((unused)) = va_arg(ap, const char *);                                          \
          method_fn method __attribute__((unused)) = va_arg(ap, method_fn);

/**
   * Dieses Define expandiert sich im Klassen-Ctor zu der Selector-Abfrage, um Methoden zu überschreiben, bzw der Klasse hinzuzufügen.
   */
#define OVERRIDE_METHOD(PREFIX, NAME) \
      if (selector == (method_fn) PREFIX##_##NAME) {                                                                    \
        if (tag) {                                                                                                      \
          self->NAME.tag = tag;                                                                                         \
          self->NAME.selector = selector;                                                                               \
          _##PREFIX##_methods.NAME = PREFIX##_##NAME;                                                                   \
        }                                                                                                               \
        self->NAME.method = method;                                                                                     \
        continue;                                                                                                       \
      }

/**
   * Macht dasselbe wie oben, fügt die Methode nur nicht der '*_methods'-Auflistung hinzu
   */
#define OVERRIDE_METHOD_NOT_ACCESSIBLE(PREFIX, NAME) \
        if (selector == (method_fn) PREFIX##_##NAME) {                                                                  \
          if (tag) {                                                                                                    \
            self->NAME.tag = tag;                                                                                       \
            self->NAME.selector = selector;                                                                             \
          }                                                                                                             \
          self->NAME.method = method;                                                                                   \
          continue;                                                                                                     \
        }

#define CLASS_CTOR__INIT_SUPER(CLZ, SUPER) \
        } {                                                                                                             \
          _##CLZ##_methods.base = SUPER##_methods();

#define CLASS_CTOR__INIT_IFACES() \
        } {

#define CLASS_IMPLEMENT_IFACE(CLZ, IFACE) \
          IFACE##_init(&self->IFACE, app);                                                                              \
          _##CLZ##_methods.IFACE = IFACE##_methods();

#define CLASS_CTOR__END() \
        }                                                                                                               \
        va_end(ap);                                                                                                     \
        return self;                                                                                                    \
      }

/**
   * Erstellen der Funktion, die die Methoden zurueckgibt.
   *
   * Sieht dann so aus:
   *
   * CLASS_MAKE_METHODS_FUNC(lfcList);
   *    const lfcList_methods_t *lfcList_methods() {
   *      lfcList(); // simple call to init the class
   *
   *      return &_lfcList_methods;
   *    }
   */
#define CLASS_MAKE_METHODS_FUNC(CLASS) \
      const CLASS##_methods_t *CLASS##_methods() {                                                                      \
        CLASS();                                                                                                        \
        return &_##CLASS##_methods;                                                                                     \
      }
#define IFACE_MAKE_METHODS_FUNC(CLASS) \
      const CLASS##_methods_t *CLASS##_methods() {                                                                      \
        return &_##CLASS##_methods;                                                                                     \
      }

/**
   * Macro zum Erstellen der Implementierung einer globalen Funktion, die dann entweder die Klassen-Methode aufruft, oder
   * aber zu einer Funktion eines abgeleiteten Objektes forwarded.
   *
   * Beispiel an der Implementierung der Methode 'add' in einer Liste.
   *
   * Implementierung:
   *     MAKE_ONE_PARAM(lfcList, add, void *)
   *
   * Und das macht der Präprozessor draus:
   *     int lfcList_add(
   *       lfcList_t *_self,
   *       void *param
   *     ) {
   *       int result;
   *       const lfcList_class_t *class = (const lfcList_class_t *)getClass(_self);
   *       lfcList_t *self = asInstanceOf(lfcList(), _self);
   *
   *       if (isInstanceOf(lfcList_class(), class) && class->add.method) {
   *         result = ((int (*) (lfcList_t *, void *))class->add.method)(self, param);
   *       } else {
   *         lfcObject_forward(_self, &result, (method_fn) lfcList_add, "add", self);
   *       }
   *
   *       return result;
   *     }
   */
#define MAKE__RET__w_0PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                        \
          OBJ_TYPE *_self                                                                                               \
        ) {                                                                                                             \
          RESULT_TYPE result;                                                                                           \
          memset(&result, 0, sizeof(result));                                                                           \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                \
                                                                                                                        \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                    \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *))class->CALLING_METHOD_NAME.method)(self);                           \
          } else {                                                                                                      \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self);           \
          }                                                                                                             \
                                                                                                                        \
          return result;                                                                                                \
        }

#define MAKE__RET__w_1PARAM(METHOD_NAME, CONST_OBJ, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                        \
          CONST_OBJ OBJ_TYPE *_self,                                                                                    \
          PARAM1_TYPE param1                                                                                            \
        ) {                                                                                                             \
          RESULT_TYPE result;                                                                                           \
          memset(&result, 0, sizeof(result));                                                                           \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                \
                                                                                                                        \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                    \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE))class->CALLING_METHOD_NAME.method)(self, param1);      \
          } else {                                                                                                      \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1);   \
          }                                                                                                             \
                                                                                                                        \
          return result;                                                                                                \
        }

#define MAKE__RET__w_2PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                            \
          OBJ_TYPE *_self,                                                                                                                  \
          PARAM1_TYPE param1,                                                                                                               \
          PARAM2_TYPE param2                                                                                                                \
        ) {                                                                                                                                 \
          RESULT_TYPE result;                                                                                                               \
          memset(&result, 0, sizeof(result));                                                                                               \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                                    \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                                    \
                                                                                                                                            \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                        \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, PARAM2_TYPE))class->CALLING_METHOD_NAME.method)(self, param1, param2);     \
          } else {                                                                                                                          \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, param2);               \
          }                                                                                                                                 \
                                                                                                                                            \
          return result;                                                                                                                    \
        }

#define MAKE__RET__w_2PARAM_VA(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                    \
          OBJ_TYPE *_self,                                                                                                          \
          PARAM1_TYPE param1,                                                                                                       \
          ...                                                                                                                       \
        ) {                                                                                                                         \
          RESULT_TYPE result;                                                                                                       \
          memset(&result, 0, sizeof(result));                                                                                       \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                            \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                            \
          va_list va;                                                                                                               \
          va_start(va, param1);                                                                                                     \
                                                                                                                                    \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, va_list))class->CALLING_METHOD_NAME.method)(self, param1, va);     \
          } else {                                                                                                                  \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, va);           \
          }                                                                                                                         \
          va_end(va);                                                                                                               \
                                                                                                                                    \
          return result;                                                                                                            \
        }

#define MAKE__RET__w_3PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                                                \
          OBJ_TYPE *_self,                                                                                                                                      \
          PARAM1_TYPE param1,                                                                                                                                   \
          PARAM2_TYPE param2,                                                                                                                                   \
          PARAM3_TYPE param3                                                                                                                                    \
        ) {                                                                                                                                                     \
          RESULT_TYPE result;                                                                                                                                   \
          memset(&result, 0, sizeof(result));                                                                                                                   \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                                                        \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                                                        \
                                                                                                                                                                \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                                            \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE))class->CALLING_METHOD_NAME.method)(self, param1, param2, param3);    \
          } else {                                                                                                                                              \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, param2, param3);                           \
          }                                                                                                                                                     \
                                                                                                                                                                \
          return result;                                                                                                                                        \
        }

#define MAKE__RET__w_4PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                                                        \
          OBJ_TYPE *_self,                                                                                                                                              \
          PARAM1_TYPE param1,                                                                                                                                           \
          PARAM2_TYPE param2,                                                                                                                                           \
          PARAM3_TYPE param3,                                                                                                                                           \
          PARAM4_TYPE param4                                                                                                                                            \
        ) {                                                                                                                                                             \
          RESULT_TYPE result;                                                                                                                                           \
          memset(&result, 0, sizeof(result));                                                                                                                           \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                                                                \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                                                                \
                                                                                                                                                                        \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                                                    \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE))class->CALLING_METHOD_NAME.method)(self, param1, param2, param3, param4);    \
          } else {                                                                                                                                                      \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, param2, param3, param4);                           \
          }                                                                                                                                                             \
                                                                                                                                                                        \
          return result;                                                                                                                                                \
        }

#define MAKE__RET__w_5PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                                                        \
          OBJ_TYPE *_self,                                                                                                                                              \
          PARAM1_TYPE param1,                                                                                                                                           \
          PARAM2_TYPE param2,                                                                                                                                           \
          PARAM3_TYPE param3,                                                                                                                                           \
          PARAM4_TYPE param4,                                                                                                                                           \
          PARAM5_TYPE param5                                                                                                                                            \
        ) {                                                                                                                                                             \
          RESULT_TYPE result;                                                                                                                                           \
          memset(&result, 0, sizeof(result));                                                                                                                           \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                                                                \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                                                                \
                                                                                                                                                                        \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                                                    \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE))class->CALLING_METHOD_NAME.method)(self, param1, param2, param3, param4, param5); \
          } else {                                                                                                                                                      \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, param2, param3, param4, param5); \
          }                                                                                                                                                             \
                                                                                                                                                                        \
          return result;                                                                                                                                                \
        }

#define MAKE__RET__w_8PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                                                        \
          OBJ_TYPE *_self,                                                                                                                                              \
          PARAM1_TYPE param1,                                                                                                                                           \
          PARAM2_TYPE param2,                                                                                                                                           \
          PARAM3_TYPE param3,                                                                                                                                           \
          PARAM4_TYPE param4,                                                                                                                                           \
          PARAM5_TYPE param5,                                                                                                                                           \
          PARAM6_TYPE param6,                                                                                                                                           \
          PARAM7_TYPE param7,                                                                                                                                           \
          PARAM8_TYPE param8                                                                                                                                            \
        ) {                                                                                                                                                             \
          RESULT_TYPE result;                                                                                                                                           \
          memset(&result, 0, sizeof(result));                                                                                                                           \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                                                                \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                                                                \
                                                                                                                                                                        \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                                                    \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE))class->CALLING_METHOD_NAME.method)(self, param1, param2, param3, param4, param5, param6, param7, param8); \
          } else {                                                                                                                                                      \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, param2, param3, param4, param5, param6, param7, param8); \
          }                                                                                                                                                             \
                                                                                                                                                                        \
          return result;                                                                                                                                                \
        }

#define MAKE__RET__w_9PARAM(METHOD_NAME, OBJ_TYPE, CLASS_TYPE, OBJ_CLZ, CLASS_CLZ, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE, PARAM9_TYPE) \
        RESULT_TYPE METHOD_NAME(                                                                                                                                        \
          OBJ_TYPE *_self,                                                                                                                                              \
          PARAM1_TYPE param1,                                                                                                                                           \
          PARAM2_TYPE param2,                                                                                                                                           \
          PARAM3_TYPE param3,                                                                                                                                           \
          PARAM4_TYPE param4,                                                                                                                                           \
          PARAM5_TYPE param5,                                                                                                                                           \
          PARAM6_TYPE param6,                                                                                                                                           \
          PARAM7_TYPE param7,                                                                                                                                           \
          PARAM8_TYPE param8,                                                                                                                                           \
          PARAM9_TYPE param9                                                                                                                                            \
        ) {                                                                                                                                                             \
          RESULT_TYPE result;                                                                                                                                           \
          memset(&result, 0, sizeof(result));                                                                                                                           \
          const CLASS_TYPE *class = (const CLASS_TYPE *)getClass(_self);                                                                                                \
          OBJ_TYPE *self = asInstanceOf(OBJ_CLZ, _self);                                                                                                                \
                                                                                                                                                                        \
          if (isInstanceOf(CLASS_CLZ, class) && class->CALLING_METHOD_NAME.method) {                                                                                    \
            result = ((RESULT_TYPE (*) (OBJ_TYPE *, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE, PARAM9_TYPE))class->CALLING_METHOD_NAME.method)(self, param1, param2, param3, param4, param5, param6, param7, param8, param9); \
          } else {                                                                                                                                                      \
            lfcObject_forward(_self, &result, (method_fn) METHOD_NAME, STRINGIZE(CALLING_METHOD_NAME), self, param1, param2, param3, param4, param5, param6, param7, param8, param9); \
          }                                                                                                                                                             \
                                                                                                                                                                        \
          return result;                                                                                                                                                \
        }


#define IMPL_API__wRET__w_0PARAM(PREFIX, METHOD, RESULT_TYPE) \
      MAKE__RET__w_0PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE)
#define IMPL_API__wRET__w_1PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE) \
      MAKE__RET__w_1PARAM(PREFIX##_##METHOD,   , PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE)
#define IMPL_API__wRET__w_2PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE) \
      MAKE__RET__w_2PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE)
#define IMPL_API__wRET__w_2PARAM_VA(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE) \
      MAKE__RET__w_2PARAM_VA(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE)
#define IMPL_API__wRET__w_3PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE) \
      MAKE__RET__w_3PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE)
#define IMPL_API__wRET__w_4PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE) \
      MAKE__RET__w_4PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE)
#define IMPL_API__wRET__w_5PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE) \
      MAKE__RET__w_5PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE)
#define IMPL_API__wRET__w_8PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE) \
      MAKE__RET__w_8PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE)
#define IMPL_API__wRET__w_9PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE, PARAM9_TYPE) \
      MAKE__RET__w_9PARAM(PREFIX##_##METHOD, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE, PARAM5_TYPE, PARAM6_TYPE, PARAM7_TYPE, PARAM8_TYPE, PARAM9_TYPE)

#define CONST_IMPL_API__wRET__w_1PARAM(PREFIX, METHOD, RESULT_TYPE, PARAM1_TYPE) \
      MAKE__RET__w_1PARAM(PREFIX##_##METHOD, const, PREFIX##_t, PREFIX##_class_t, PREFIX(), PREFIX##_class(), METHOD, RESULT_TYPE, PARAM1_TYPE)

/**
   * Macro zum Erstellen der Implementierung einer globalen Funktion, die dann entweder die super-Methode aufruft.
   *
   * Beispiel an der Implementierung der Methode 'add' in einer Liste.
   *
   * Implementierung:
   *     SUPER__RET__w_1PARAM(lfcObject, vsuper_ctor, ctor, void *, va_list *)
   *
   * Und das macht der Präprozessor draus:
   *     void *lfcObject_vsuper_ctor(const void *_class, void *_self, va_list *app) {
   *       const struct lfcObject_class *superclass = super(_class);
   *
   *       assert(superclass->ctor.method);
   *       return ((void *(*)(const void *, va_list *))superclass->ctor.method)(_self, app);
   *     }
   */
#define MAKE_SUPER__RET__w_0PARAM(METHOD_NAME, CALLING_METHOD_NAME, RESULT_TYPE) \
      RESULT_TYPE METHOD_NAME(                                                                                      \
        const void *_class,                                                                                         \
        void *_self                                                                                                 \
      ) {                                                                                                           \
        const lfcObject_class_t *superclass = super(_class);                                                        \
                                                                                                                    \
        if (superclass == NULL) return (RESULT_TYPE)0;                                                              \
        if (!superclass->CALLING_METHOD_NAME.method) return (RESULT_TYPE)0;                                         \
                                                                                                                    \
        return ((RESULT_TYPE(*)(const void *))superclass->CALLING_METHOD_NAME.method)(_self);                       \
      }
#define MAKE_SUPER__RET__w_1PARAM(METHOD_NAME, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE) \
      RESULT_TYPE METHOD_NAME(                                                                                      \
        const void *_class,                                                                                         \
        void *_self,                                                                                                \
        PARAM1_TYPE param1                                                                                          \
      ) {                                                                                                           \
        const lfcObject_class_t *superclass = super(_class);                                                        \
                                                                                                                    \
        if (superclass == NULL) return (RESULT_TYPE)0;                                                              \
        if (!superclass->CALLING_METHOD_NAME.method) return (RESULT_TYPE)0;                                         \
                                                                                                                    \
        return ((RESULT_TYPE(*)(const void *, PARAM1_TYPE))superclass->CALLING_METHOD_NAME.method)(_self, param1);  \
      }
#define MAKE_SUPER__RET__w_4PARAM(METHOD_NAME, CALLING_METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE) \
      RESULT_TYPE METHOD_NAME(                                                                                      \
        const void *_class,                                                                                         \
        void *_self,                                                                                                \
        PARAM1_TYPE param1,                                                                                         \
        PARAM2_TYPE param2,                                                                                         \
        PARAM3_TYPE param3,                                                                                         \
        PARAM4_TYPE param4                                                                                          \
      ) {                                                                                                           \
        const lfcObject_class_t *superclass = super(_class);                                                        \
                                                                                                                    \
        if (superclass == NULL) return (RESULT_TYPE)0;                                                              \
        if (!superclass->CALLING_METHOD_NAME.method) return (RESULT_TYPE)0;                                         \
                                                                                                                    \
        return ((RESULT_TYPE(*)(const void *, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE))superclass->CALLING_METHOD_NAME.method)(_self, param1, param2, param3, param4);  \
      }

#define SUPER__RET__w_0PARAM(PREFIX, METHOD, METHOD_NAME, RESULT_TYPE) \
        MAKE_SUPER__RET__w_0PARAM(PREFIX##_##METHOD, METHOD_NAME, RESULT_TYPE)

#define SUPER__RET__w_1PARAM(PREFIX, METHOD, METHOD_NAME, RESULT_TYPE, PARAM1_TYPE) \
        MAKE_SUPER__RET__w_1PARAM(PREFIX##_##METHOD, METHOD_NAME, RESULT_TYPE, PARAM1_TYPE)

#define SUPER__RET__w_4PARAM(PREFIX, METHOD, METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE) \
        MAKE_SUPER__RET__w_4PARAM(PREFIX##_##METHOD, METHOD_NAME, RESULT_TYPE, PARAM1_TYPE, PARAM2_TYPE, PARAM3_TYPE, PARAM4_TYPE)

/******************************************************************************************/
/* MACROS FOR EASE CLASS DEFINITION (w/o ifaces)                                          */
/******************************************************************************************/
#define lfcCLASS_VA_NARGS_IMPL(_1, _2, _3,        \
                               _4, _5, _6,        \
                               _7, _8, _9,        \
                               _10, _11, _12,     \
                               _13, _14, _15,     \
                               _16, _17, _18,     \
                               _19, _20, _21,     \
                               _22, _23, _24,     \
                               _25, _26, _27,     \
                               _28, _29, _30,     \
                               _31, _32, _33,     \
                               N, ...)                                          N
#define lfcCLASS_VA_NARGS(...)                                                  lfcCLASS_VA_NARGS_IMPL(X,##__VA_ARGS__, \
                                                                                       0, 0, 10,  \
                                                                                       0, 0,  9,  \
                                                                                       0, 0,  8,  \
                                                                                       0, 0,  7,  \
                                                                                       0, 0,  6,  \
                                                                                       0, 0,  5,  \
                                                                                       0, 0,  4,  \
                                                                                       0, 0,  3,  \
                                                                                       0, 0,  2,  \
                                                                                       0, 0,  1,  \
                                                                                       0, 0,  0)
#define lfcCLASS_VARARG_IMPL2(base, count, name, super, fields, ...)            base##count(name, super, fields, __VA_ARGS__)
#define lfcCLASS_VARARG_IMPL(base, count, name, super, fields, ...)             lfcCLASS_VARARG_IMPL2(base, count, name, super, fields, __VA_ARGS__)
#define lfcCLASS_VARARG(base, name, super, fields, ...)                         lfcCLASS_VARARG_IMPL(base, lfcCLASS_VA_NARGS(__VA_ARGS__), name, super, fields, __VA_ARGS__)

#define lfcDEFINE_CLASS(name, super, fields, ...)                               lfcCLASS_VARARG(lfcCLASS_noIface, name, super, fields, __VA_ARGS__)

#define lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields) \
    struct name { const super##_t _; \
        fields \
    }; \

#define lfcCLASS_noIface1(name, super, fields,          \
    _1Return, _1Name, _1Args                            \
)                                                   \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;

#define lfcCLASS_noIface2(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args                            \
)                                                   \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;

#define lfcCLASS_noIface3(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args                            \
)                                                   \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;

#define lfcCLASS_noIface4(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args,                           \
    _4Return, _4Name, _4Args                            \
)                                                   \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
        method_t _4Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
        _4Return (*_4Name)_4Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;                    \
    _4Return name##_##_4Name _4Args;

#define lfcCLASS_noIface5(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args,                           \
    _4Return, _4Name, _4Args,                           \
    _5Return, _5Name, _5Args                            \
)                                                       \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
        method_t _4Name;                                \
        method_t _5Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
        _4Return (*_4Name)_4Args;                       \
        _5Return (*_5Name)_5Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;                    \
    _4Return name##_##_4Name _4Args;                    \
    _5Return name##_##_5Name _5Args;

#define lfcCLASS_noIface6(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args,                           \
    _4Return, _4Name, _4Args,                           \
    _5Return, _5Name, _5Args,                           \
    _6Return, _6Name, _6Args                            \
)                                                       \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
        method_t _4Name;                                \
        method_t _5Name;                                \
        method_t _6Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
        _4Return (*_4Name)_4Args;                       \
        _5Return (*_5Name)_5Args;                       \
        _6Return (*_6Name)_6Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;                    \
    _4Return name##_##_4Name _4Args;                    \
    _5Return name##_##_5Name _5Args;                    \
    _6Return name##_##_6Name _6Args;

#define lfcCLASS_noIface7(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args,                           \
    _4Return, _4Name, _4Args,                           \
    _5Return, _5Name, _5Args,                           \
    _6Return, _6Name, _6Args,                           \
    _7Return, _7Name, _7Args                            \
)                                                       \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
        method_t _4Name;                                \
        method_t _5Name;                                \
        method_t _6Name;                                \
        method_t _7Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
        _4Return (*_4Name)_4Args;                       \
        _5Return (*_5Name)_5Args;                       \
        _6Return (*_6Name)_6Args;                       \
        _7Return (*_6Name)_7Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;                    \
    _4Return name##_##_4Name _4Args;                    \
    _5Return name##_##_5Name _5Args;                    \
    _6Return name##_##_6Name _6Args;                    \
    _7Return name##_##_7Name _7Args;

#define lfcCLASS_noIface8(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args,                           \
    _4Return, _4Name, _4Args,                           \
    _5Return, _5Name, _5Args,                           \
    _6Return, _6Name, _6Args,                           \
    _7Return, _7Name, _7Args,                           \
    _8Return, _8Name, _8Args                            \
)                                                       \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
        method_t _4Name;                                \
        method_t _5Name;                                \
        method_t _6Name;                                \
        method_t _7Name;                                \
        method_t _8Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
        _4Return (*_4Name)_4Args;                       \
        _5Return (*_5Name)_5Args;                       \
        _6Return (*_6Name)_6Args;                       \
        _7Return (*_6Name)_7Args;                       \
        _8Return (*_8Name)_8Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;                    \
    _4Return name##_##_4Name _4Args;                    \
    _5Return name##_##_5Name _5Args;                    \
    _6Return name##_##_6Name _6Args;                    \
    _7Return name##_##_7Name _7Args;                    \
    _8Return name##_##_8Name _8Args;

#define lfcCLASS_noIface9(name, super, fields,          \
    _1Return, _1Name, _1Args,                           \
    _2Return, _2Name, _2Args,                           \
    _3Return, _3Name, _3Args,                           \
    _4Return, _4Name, _4Args,                           \
    _5Return, _5Name, _5Args,                           \
    _6Return, _6Name, _6Args,                           \
    _7Return, _7Name, _7Args,                           \
    _8Return, _8Name, _8Args,                           \
    _9Return, _9Name, _9Args                            \
)                                                       \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t _1Name;                                \
        method_t _2Name;                                \
        method_t _3Name;                                \
        method_t _4Name;                                \
        method_t _5Name;                                \
        method_t _6Name;                                \
        method_t _7Name;                                \
        method_t _8Name;                                \
        method_t _9Name;                                \
    };                                                  \
    struct name##_methods {                             \
        _1Return (*_1Name)_1Args;                       \
        _2Return (*_2Name)_2Args;                       \
        _3Return (*_3Name)_3Args;                       \
        _4Return (*_4Name)_4Args;                       \
        _5Return (*_5Name)_5Args;                       \
        _6Return (*_6Name)_6Args;                       \
        _7Return (*_6Name)_7Args;                       \
        _8Return (*_8Name)_8Args;                       \
        _9Return (*_9Name)_9Args;                       \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
    _1Return name##_##_1Name _1Args;                    \
    _2Return name##_##_2Name _2Args;                    \
    _3Return name##_##_3Name _3Args;                    \
    _4Return name##_##_4Name _4Args;                    \
    _5Return name##_##_5Name _5Args;                    \
    _6Return name##_##_6Name _6Args;                    \
    _7Return name##_##_7Name _7Args;                    \
    _8Return name##_##_8Name _8Args;                    \
    _9Return name##_##_9Name _9Args;

#define lfcCLASS_noIface10(name, super, fields,         \
     _1Return,  _1Name,  _1Args,                        \
     _2Return,  _2Name,  _2Args,                        \
     _3Return,  _3Name,  _3Args,                        \
     _4Return,  _4Name,  _4Args,                        \
     _5Return,  _5Name,  _5Args,                        \
     _6Return,  _6Name,  _6Args,                        \
     _7Return,  _7Name,  _7Args,                        \
     _8Return,  _8Name,  _8Args,                        \
     _9Return,  _9Name,  _9Args,                        \
    _10Return, _10Name, _10Args                         \
)                                                       \
    lfcDEFINE_CLASS__OBJECTSTRUCT(name, super, fields)  \
    struct name##_class { const super##_class_t _;      \
        method_t  _1Name;                               \
        method_t  _2Name;                               \
        method_t  _3Name;                               \
        method_t  _4Name;                               \
        method_t  _5Name;                               \
        method_t  _6Name;                               \
        method_t  _7Name;                               \
        method_t  _8Name;                               \
        method_t  _9Name;                               \
        method_t _10Name;                               \
    };                                                  \
    struct name##_methods {                             \
         _1Return (* _1Name) _1Args;                    \
         _2Return (* _2Name) _2Args;                    \
         _3Return (* _3Name) _3Args;                    \
         _4Return (* _4Name) _4Args;                    \
         _5Return (* _5Name) _5Args;                    \
         _6Return (* _6Name) _6Args;                    \
         _7Return (* _6Name) _7Args;                    \
         _8Return (* _8Name) _8Args;                    \
         _9Return (* _9Name) _9Args;                    \
        _10Return (*_10Name)_10Args;                    \
                                                        \
        const super##_methods_t *super;                 \
    };                                                  \
     _1Return name##_##_1Name _1Args;                   \
     _2Return name##_##_2Name _2Args;                   \
     _3Return name##_##_3Name _3Args;                   \
     _4Return name##_##_4Name _4Args;                   \
     _5Return name##_##_5Name _5Args;                   \
     _6Return name##_##_6Name _6Args;                   \
     _7Return name##_##_7Name _7Args;                   \
     _8Return name##_##_8Name _8Args;                   \
     _9Return name##_##_9Name _9Args;                   \
    _10Return name##_##_10Name _10Args;

#endif //LIBFORC_CORE_LFCOBJECT_H
