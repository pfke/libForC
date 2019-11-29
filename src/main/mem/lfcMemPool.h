#ifndef LIBFORC_MEM_MEMPOOL_H
#define LIBFORC_MEM_MEMPOOL_H

  #include <stdint.h>
  #include <pthread.h>
  #include "../core/lfcObject.h"

  #define DEFAULT_LIST_INITIAL_SIZE         10
  #define DEFAULT_LIST_INCREASE_BY          10
  #define LFCMEMPOOL_MAGIC                  0x2a36
  #define LFCMEMPOOL_INVALID_MEM_PATTERN    0xaa

  typedef enum memPool_mode memPool_mode_e;
  enum memPool_mode {
      memPool_READ_ONLY,
      memPool_WRITE_W_INITIALIZE,
      memPool_WRITE_WO_INITIALIZE,
  };

  /**
   * Diese Header-Struktur darf nicht packed sein, weil es dadurch mit den nachfolgenden Daten Probleme mit dem
   * Alignement gibt.
   * Wenn Mips oder so auf unaligned Zugriffe stösst, gibt es Performance-Einbussen um den Faktor 1000.
   */
  typedef struct memPool_headr memPool_headr_t;
  struct memPool_headr {
      uint16_t magic;
      uint8_t isUsed;
      uint32_t len;             // length w/o header
      uint32_t prev_len;        // length of the block before us (w/o header)

      char data[0] __attribute__ ((aligned));
  };

  DEFINE_CLASS(lfcMemPool)

  struct lfcMemPool { const struct lfcObject _;
    void *mem_ptr;
    size_t usable_size;

    size_t used_block_count;
    size_t used_size;
    size_t used_data_size;
    size_t highwatermark_used_size;

    pthread_mutex_t lock;

    memPool_mode_e mode;
  };

  struct lfcMemPool_class { const struct lfcObject_class _;
    method_t alloc;
    method_t calloc;
    method_t free;

    method_t defrag;

    method_t getBlockCount;
    method_t getBlockSize;
    method_t getFreeDataSize;

    method_t next;
    method_t prev;

    method_t dump_va;
    method_t isValid;
  };

  struct lfcMemPool_methods {
    void  *(*alloc       )(lfcMemPool_t *self, uint16_t needed_size);
    void  *(*calloc      )(lfcMemPool_t *self, uint16_t needed_size);
    int    (*free        )(lfcMemPool_t *self, void *ptr);

    /**
      * Defragmentieren den Scheiss.
      * Auf eigene Gefahr! Niemand darf mehr einen ptr auf einen Block halten!
      */
    int    (*defrag      )(lfcMemPool_t *self);

    size_t (*getBlockCount     )(lfcMemPool_t *self);
    size_t (*getBlockSize   )(lfcMemPool_t *self, void *ptr);
    size_t (*getFreeDataSize)(lfcMemPool_t *self);

    void  *(*next        )(lfcMemPool_t *self, const void *current);
    void  *(*prev        )(lfcMemPool_t *self, const void *current);

    int     (*dump_va     )(lfcMemPool_t *self, size_t cols, char space, FILE *fp, const char *hdr_format, va_list hdr_format_args);
    uint8_t (*isValid     )(lfcMemPool_t *self, char **diagnostics);

    // super
    const lfcObject_methods_t *base;

    // ifaces
  };

  lfcMemPool_t *lfcMemPool_ctor(char *mem_ptr, size_t usable_size, memPool_mode_e mode);

  void *lfcMemPool_alloc(lfcMemPool_t *self, uint16_t needed_size);
  void *lfcMemPool_calloc(lfcMemPool_t *self, uint16_t needed_size);
  int   lfcMemPool_free(lfcMemPool_t *self, void *ptr);

  int   lfcMemPool_defrag(lfcMemPool_t *self);

  /**
   * Gibt die Anzahl der genutzten Blöcke zurueck.
   */
  size_t lfcMemPool_getBlockCount(lfcMemPool_t *self);

  size_t lfcMemPool_getBlockSize(lfcMemPool_t *self, void *ptr);
  /**
   * Diese MEthode gibt die noch vorhandene nutzbare Anzahl an Bytes zurück.
   * Diese ist aber sehr wahrscheinlich fragmentiert.
   */
  size_t lfcMemPool_getFreeDataSize(lfcMemPool_t *self);

  /**
   * Iteriert über alle genutzte items.
   */
  void *lfcMemPool_next(lfcMemPool_t *self, const void *);
  void *lfcMemPool_prev(lfcMemPool_t *self, const void *);

  /**
   * Zu dbg-Zwecken den kompletten mem dumpen.
   *
   * @param self ist unsere Instanz
   * @param cols Anzahl Spalten
   * @param space das Zeichen zwischen den einzelnen Spalten
   * @param fp hierhin soll gedruckt werden
   * @param format header string, kann NULL sein
   * @param format_args va-liste für den header-String
   * @return 0 .. alles ok
   *          -EINVAL .. fp fehlt
   */
  int lfcMemPool_dump_va (
      lfcMemPool_t *self,
      size_t cols,
      char space,
      FILE *fp,
      const char *hdr_format,
      va_list hdr_format_args
  );
  int lfcMemPool_dump (
      lfcMemPool_t *self,
      size_t cols,
      char space,
      FILE *fp,
      const char *hdr_format,
      ...
  );

  uint8_t lfcMemPool_isValid(lfcMemPool_t *self, char **);


#endif //LIBFORC_MEM_MEMPOOL_H
