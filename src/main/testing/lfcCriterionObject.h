#ifndef LIBFORC_TESTINGLFCCRITERIONOBJECT_H_H
#define LIBFORC_TESTINGLFCCRITERIONOBJECT_H_H

  /**
   * Dieses Macro erstellt eine Funktion in der das Vorhandensein und das korrekte Setzen von Klassenmethoden geprüft wird.
   */
  #define TEST_CLASS_METHODS(CLZ, METHOD)                                                                                                     \
      Test(                                                                                                                                   \
        TEST_SUITE_NAME,                                                                                                                      \
        check_method_##METHOD                                                                                                                 \
      ) {                                                                                                                                     \
        should_be_same_ptr_wText(CLZ##_methods()->METHOD, CLZ##_##METHOD, "method '" STRINGIZE(CLZ) "_methods()->" STRINGIZE(METHOD) "' not set");  \
      }

  #define TEST_CLASS_SUPER_METHODS(CLZ, SUPER)                                                                          \
      Test(                                                                                                             \
        TEST_SUITE_NAME,                                                                                                \
        check_super_method_##SUPER                                                                                      \
      ) {                                                                                                               \
        should_be_same_ptr_wText(CLZ##_methods()->base, SUPER##_methods(), "method '" STRINGIZE(CLZ) "_methods()->base' not set"); \
      }

  #define TEST_CLASS_IFACE_METHODS(CLZ, IFACE)                                                                          \
      Test(                                                                                                             \
        TEST_SUITE_NAME,                                                                                                \
        check_iface_method_##IFACE                                                                                      \
      ) {                                                                                                               \
        should_be_same_ptr_wText(CLZ##_methods()->IFACE, IFACE##_methods(), "method '" STRINGIZE(CLZ) "_methods()->" STRINGIZE(IFACE) "' not set");\
      }

#endif //LIBFORC_TESTING_LFCCRITERIONOBJECT_H_H
