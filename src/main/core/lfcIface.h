#ifndef LIBFORC_CORE_LFCIFACE_H
#define LIBFORC_CORE_LFCIFACE_H

  /**
   * Macro zur Implementierung der init-Funktion eines INterfaces.
   * In dieser Init werden die Funktionen aus der new-variablen-Arg-Liste
   * herausgelesen und auf die Funktionspointer geschrieben.
   *
   * Aufruf:
   *    IFACE_INIT__START(lfcIIterable)
   *      OVERRIDE_METHOD(lfcIIterable, isEmpty)
   *    IFACE_INIT__END()
   *
   * Resultat:
   *    lfcIIterable_t *lfcIIterable_init(
   *      lfcIIterable_t *self,
   *      va_list *app
   *    ) {
   *      method_fn selector;
   *
   *      va_list ap = {0};
   *      va_copy(ap, *app);
   *
   *      while ((selector = va_arg(ap, method_fn))) {
   *        const char * tag = va_arg(ap, const char *);
   *        method_fn method = va_arg(ap, method_fn);
   *
   *        OVERRIDE_METHOD(lfcIIterable, isEmpty)
   *      }
   *      va_end(ap);
   *
   *      return self;
   *    }
   */
  #define IFACE_INIT__START(PREFIX)                                     \
      PREFIX##_t *PREFIX##_init(                                        \
        PREFIX##_t *self,                                               \
        va_list *app                                                    \
      ) {                                                               \
        method_fn selector;                                             \
                                                                        \
        va_list ap = {0};                                               \
        va_copy(ap, *app);                                              \
                                                                        \
        while ((selector = va_arg(ap, method_fn))) {                    \
          const char * tag = va_arg(ap, const char *);                  \
          method_fn method = va_arg(ap, method_fn);

  #define IFACE_INIT__END()                                             \
        }                                                               \
        va_end(ap);                                                     \
                                                                        \
        return self;                                                    \
      }

  /**
   * Dieses Macro erstellt eine Interface-Funktion, die beim Aufruf die von der Klasse
   * entsprechend überschriebene Methode sucht und aufruft.
   *
   * Aufruf:
   *    IFACE_FORWARD__RET__w_0Param(lfcIIterable, isEmpty, int)
   * Resultat:
   *    int lfcIIterable_isEmpty(
   *      void *_self
   *    ) {
   *      const method_t *method = respondsTo(_self, "isEmpty");
   *
   *      assert(method);
   *      assert(method->selector == (method_fn)lfcIIterable_isEmpty);
   *
   *      return ((int(*)(void *))method->method)(_self);
   *    }
   */
  #define IFACE_FORWARD__RET__w_0Param(PREFIX, METHOD, RETURN_TYPE)                               \
      RETURN_TYPE PREFIX##_##METHOD(                                                              \
        void *_self                                                                               \
      ) {                                                                                         \
        const method_t *method = respondsTo(_self, STRINGIZE(METHOD));                            \
                                                                                                  \
        if (!method) { return (RETURN_TYPE)0; }                                                   \
        if (!(method->selector == (method_fn)PREFIX##_##METHOD)) { return (RETURN_TYPE)0; }       \
                                                                                                  \
        return ((RETURN_TYPE(*)(void *))method->method)(_self);                                   \
      }

  #define IFACE_FORWARD__RET__w_1Param(PREFIX, METHOD, RETURN_TYPE, PARAM1_TYPE)                  \
      RETURN_TYPE PREFIX##_##METHOD(                                                              \
        void *_self,                                                                              \
        PARAM1_TYPE param1                                                                        \
      ) {                                                                                         \
        const method_t *method = respondsTo(_self, STRINGIZE(METHOD));                            \
                                                                                                  \
        if (!method) { return (RETURN_TYPE)0; }                                                   \
        if (!(method->selector == (method_fn)PREFIX##_##METHOD)) { return (RETURN_TYPE)0; }       \
                                                                                                  \
        return ((RETURN_TYPE(*)(void *, PARAM1_TYPE))method->method)(_self, param1);              \
      }

  #define IFACE_FORWARD__RET__w_2Param(PREFIX, METHOD, RETURN_TYPE, PARAM1_TYPE, PARAM2_TYPE)             \
      RETURN_TYPE PREFIX##_##METHOD(                                                                      \
        void *_self,                                                                                      \
        PARAM1_TYPE param1,                                                                               \
        PARAM2_TYPE param2                                                                                \
      ) {                                                                                                 \
        const method_t *method = respondsTo(_self, STRINGIZE(METHOD));                                    \
                                                                                                          \
        if (!method) { return (RETURN_TYPE)0; }                                                           \
        if (!(method->selector == (method_fn)PREFIX##_##METHOD)) { return (RETURN_TYPE)0; }               \
                                                                                                          \
        return ((RETURN_TYPE(*)(void *, PARAM1_TYPE, PARAM2_TYPE))method->method)(_self, param1, param2); \
      }

#endif //LIBFORC_CORE_LFCIFACE_H
