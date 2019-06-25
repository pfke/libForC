#ifndef LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_
#define LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_

  #include "../../core/lfcCore.h"
  #include "../../core/lfcObject.h"
  #include "lfcLogCommon.h"

  typedef struct lfcLogAppender_methods lfcLogAppender_methods_t;

  typedef struct lfcLogAppender {
      method_t put;
  } lfcLogAppender_t;

  struct lfcLogAppender_methods {
      int (*put)(void *_self, lfcLog_t *log);
  };

  lfcLogAppender_t * lfcLogAppender_init(lfcLogAppender_t *iface, va_list *app);
  extern const lfcLogAppender_methods_t *lfcLogAppender_methods(void);

  /**
   * Print log item.
   */
  int lfcLogAppender_put(void *_self, lfcLog_t *log);

#endif //LIBFORC_TOOLS_LOGGING_LOGAPPENDER_H_
