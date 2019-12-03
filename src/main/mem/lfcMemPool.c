#include "lfcMemPool.h"

#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
#define IS_USED     1
#define NOT_USED    0

// uncomment to enable validation of memory pool blocks. Diagnostics will be given on stderr.
//#define VALIDATE_MEMPOOL

static const lfcMemPool_t *_lfcMemPool;
static const lfcMemPool_class_t *_lfcMemPool_class;
static lfcMemPool_methods_t _lfcMemPool_methods;

/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static int private_lfcMemPool_isOurMem(lfcMemPool_t *self, const void *ptr);

static lfcMemPool_t *public_lfcMemPool_ctor         (void *_self, va_list *app);
static lfcMemPool_t *public_lfcMemPool_dtor         (lfcMemPool_t *self);
static void*         public_lfcMemPool_alloc        (lfcMemPool_t *self, uint16_t needed_size);
static void*         public_lfcMemPool_calloc       (lfcMemPool_t *self, uint16_t needed_size);
static int           public_lfcMemPool_free         (lfcMemPool_t *self, void *ptr);
static size_t        public_lfcMemPool_getBlockSize (lfcMemPool_t *self, void *ptr);
static void*         public_lfcMemPool_next         (lfcMemPool_t *self, const void *current);
static void*         public_lfcMemPool_prev         (lfcMemPool_t *self, const void *current);
       int                  lfcMemPool_dump         (lfcMemPool_t *self, size_t cols, char space, FILE *fp, const char *hdr_format, ...);
static int           public_lfcMemPool_dump_va      (lfcMemPool_t *self, size_t cols, char space, FILE *fp, const char *hdr_format, va_list hdr_format_args);
static uint8_t       public_lfcMemPool_isValid      (lfcMemPool_t *self, char **diagnostics);

/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

#ifdef VALIDATE_MEMPOOL
// Die Funktion gibt Infos zur Korrektheit des Mempools aus. Debugging-Wrapper für public_lfcMemPool_isValid.
    static void private_lfcMemPool_validate(lfcMemPool_t *self, char *where) {
        char *diagnostics = NULL;
        if (!public_lfcMemPool_isValid(self, &diagnostics)) {
            fprintf(stderr, "%s: Memory pool invalid. Diagnostics follow. \n%s", where, diagnostics);
        } else {
            fprintf(stderr, "%s: Memory ok. \n", where);
        }
        free(diagnostics);
    }
#else
#define private_lfcMemPool_validate(...)
#endif

/**
 * Gibt das nächste Element der Liste zurueck
 */
static void *private_lfcMemPool_nextAll_nolock(
    lfcMemPool_t *self,
    const void *current
) {
    void *next_ptr = NULL;

    if (!private_lfcMemPool_isOurMem(self, current) && current != NULL) return NULL;

    // nächsten header suchen
    const memPool_headr_t *current_hdr = (memPool_headr_t *)(current - sizeof(memPool_headr_t));
    const memPool_headr_t *next_hdr = (current) ? (memPool_headr_t *)(((char *)current - sizeof(memPool_headr_t)) + current_hdr->len + sizeof(memPool_headr_t)) : self->mem_ptr;
    next_ptr = (char *)next_hdr + sizeof(memPool_headr_t);

    return private_lfcMemPool_isOurMem(self, next_ptr) ? next_ptr : NULL;
}

/**
 * Gibt den letzten Block des pool zurueck
 */
static memPool_headr_t *private_lfcMemPool_get_last_block(
    lfcMemPool_t *self
) {
    void *current = NULL, *prev = NULL;

    while (NULL != (current = private_lfcMemPool_nextAll_nolock(self, current))) {
        prev = current;
    }

    return (prev) ? (prev - sizeof(memPool_headr_t)) : NULL;
}

/**
 * Löscht die ptr aus der Liste.
 *
 * @param _self die Instanz der Liste selbst
 */
static int private_lfcMemPool_free_nolock(
    lfcMemPool_t *self,
    void *ptr
) {
    if (!ptr) { return -EINVAL; }
    if (!private_lfcMemPool_isOurMem(self, ptr)) { return -EINVAL; }
    if (memPool_READ_ONLY == self->mode) { return -EACCES; }

    private_lfcMemPool_validate(self, "public_lfcMemPool_free begin");

    memPool_headr_t *hdr = (memPool_headr_t *)(ptr - sizeof(memPool_headr_t));
    memPool_headr_t *next_hdr = (memPool_headr_t *)(ptr + hdr->len);
    memPool_headr_t *prev_hdr = (memPool_headr_t *)((char *)hdr - hdr->prev_len - sizeof(memPool_headr_t));

    if (hdr->isUsed) {
        hdr->isUsed = 0;
        self->used_size -= (hdr->len + sizeof(memPool_headr_t));
        self->used_data_size -= hdr->len;
        memset((char *) hdr + sizeof(memPool_headr_t), LFCMEMPOOL_INVALID_MEM_PATTERN, hdr->len); // Daten als ungueltig markieren
        self->used_block_count--;
    }

    // next free?
    if (private_lfcMemPool_isOurMem(self, next_hdr) && !next_hdr->isUsed) {
        memPool_headr_t *hdr_to_platt = next_hdr;

        hdr->len += next_hdr->len + sizeof(memPool_headr_t);

        next_hdr = (memPool_headr_t *)((char*)next_hdr + sizeof(memPool_headr_t) + next_hdr->len);
        if (private_lfcMemPool_isOurMem(self, next_hdr)) {
            next_hdr->prev_len = hdr->len;
        }

        memset(hdr_to_platt, LFCMEMPOOL_INVALID_MEM_PATTERN, sizeof(memPool_headr_t)); // Daten als ungueltig markieren
    }
    // prev free?
    if (private_lfcMemPool_isOurMem(self, prev_hdr) && !prev_hdr->isUsed) {
        prev_hdr->len += hdr->len + sizeof(memPool_headr_t);
        if (private_lfcMemPool_isOurMem(self, next_hdr)) {
            next_hdr->prev_len = prev_hdr->len;
        }

        memset(hdr, LFCMEMPOOL_INVALID_MEM_PATTERN, sizeof(memPool_headr_t)); // Daten als ungueltig markieren
    }


    private_lfcMemPool_validate(self, "public_lfcMemPool_free end");
    return 0;
}

static int private_lfcMemPool_isOurMem(
    lfcMemPool_t *self,
    const void *ptr
) {
  if ((ptr >= self->mem_ptr) && (ptr < (self->mem_ptr + self->usable_size))) {
    return 1;
  }

  return 0;
}

/**
 * Diese Funktion scant einen existierenden Speicher.
 */
static int private_lfcMemPool_scanMemory(
    lfcMemPool_t *self
) {
    self->used_block_count = 0;
    self->used_data_size = 0;
    self->used_size = 0;
    self->highwatermark_used_size = 0;

    if(0 == pthread_mutex_lock(&self->lock)) {
        // search a free header
        void *ptr = self->mem_ptr;

        while (1) {
            memPool_headr_t *hdr = (memPool_headr_t *)ptr;

            if (!private_lfcMemPool_isOurMem(self, ptr)) {
                break;
            }

            if (hdr->isUsed == 1) {
                self->used_block_count++;
                self->used_data_size += hdr->len;
                self->used_size += (hdr->len + sizeof(memPool_headr_t));
            }

            ptr = ptr + hdr->len + sizeof(memPool_headr_t);
        }

        pthread_mutex_unlock(&self->lock);
    }

    self->highwatermark_used_size = self->used_size;

    return 0;
}

static int private_lfcMemPool_updateHeader(
    lfcMemPool_t *self,
    void *ptr,
    size_t len
) {
    if (!private_lfcMemPool_isOurMem(self, ptr)) {
        fprintf(stderr, "%s: ptr out of range\n", __FUNCTION__);
        return -EINVAL;
    }
    
    memPool_headr_t *hdr = (memPool_headr_t *)ptr;
    if (!private_lfcMemPool_isOurMem(self, ptr + hdr->len - 1)) {
        fprintf(stderr, "%s: ptr end is out of range. This is a internal error.\n", __FUNCTION__);
        return -EINVAL;
    }
    
    if (hdr->len < len) {
        fprintf(stderr, "%s: memory block is too small\n", __FUNCTION__);
        return -EINVAL;
    }

    if (LFCMEMPOOL_MAGIC != hdr->magic) {
        fprintf(stderr, "%s: magic does not match: 0x%04x (0x%04x)\n", __FUNCTION__, hdr->magic, LFCMEMPOOL_MAGIC);
        return -EINVAL;
    }

    hdr->isUsed = IS_USED;
    
    if (hdr->len > len + sizeof(memPool_headr_t)) {
        // We can split the block
        unsigned int old_length = hdr->len;
        hdr->len = len;
        
        memPool_headr_t *following_block = (memPool_headr_t*)(ptr + len + sizeof(memPool_headr_t));
        following_block->magic = LFCMEMPOOL_MAGIC;
        following_block->isUsed = NOT_USED;
        following_block->len = old_length - len - sizeof(memPool_headr_t);
        following_block->prev_len = len;

        memPool_headr_t *block_after_following_block = (memPool_headr_t*)((void *)(following_block) + following_block->len + sizeof(memPool_headr_t));

        if (private_lfcMemPool_isOurMem(self, block_after_following_block)) {
            block_after_following_block->prev_len = following_block->len;
        }
    }

    return 0;
}

/******************************************************************************************/
/* PUBLIC METHODS                                                                        */
/******************************************************************************************/

/**
 * Constructor der Liste selbst.
 *
 * @param _self die bereits angelegte Instanz der Liste selbst
 * @param app ist die variable Argumenten-Liste (es werden 2 uint32_t erwartet)
 *
 * @return NULL .. wenn kein Speicher für das interne Array da ist
 *         sonst != NULL
 */
static lfcMemPool_t *public_lfcMemPool_ctor(
    void *_self,
    va_list *app
) {
    lfcMemPool_t *self = (lfcMemPool_t *) lfcObject_super_ctor(lfcMemPool(), _self);

    if (self == 0)
        return 0;

    // read args
    self->mem_ptr = va_arg(*app, char *);
    self->usable_size = va_arg(*app, size_t);
    self->mode = va_arg(*app, memPool_mode_e);

    if (!self->mem_ptr) {
        goto err;
    }
    if (!self->usable_size) {
        goto err;
    }
    // min. 1 Element muss reinpassen
    if (self->usable_size < (sizeof(memPool_headr_t) + __BIGGEST_ALIGNMENT__)) {
        goto err;
    }

    pthread_mutex_init(&self->lock, NULL);

    switch (self->mode) {
        case memPool_READ_ONLY:
        case memPool_WRITE_WO_INITIALIZE:
            private_lfcMemPool_scanMemory(self);
            break;

        case memPool_WRITE_W_INITIALIZE:
        {
            memset(self->mem_ptr, LFCMEMPOOL_INVALID_MEM_PATTERN, self->usable_size);
            memPool_headr_t *first_block = (memPool_headr_t *) self->mem_ptr;
            first_block->magic = LFCMEMPOOL_MAGIC;
            first_block->isUsed = NOT_USED;
            first_block->len = (unsigned)self->usable_size - sizeof(memPool_headr_t);
            first_block->prev_len = 0;

            private_lfcMemPool_validate(self, "public_lfcMemPool_ctor");
            break;
        }

        default:
            goto err;
    }

    return self;

err:
    free(self);

    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcMemPool_t *public_lfcMemPool_dtor(
    lfcMemPool_t *self
) {
    lfcObject_super_dtor(lfcMemPool(), self);

    pthread_mutex_destroy(&self->lock);

    return self;
}

/**
 * Diese Methode gibt einen freien Speicherblock zurueck.
 */
static void *public_lfcMemPool_alloc(
    lfcMemPool_t *self,
    uint16_t needed_size
) {
    if (!needed_size)
        return NULL;
    if (self->mode != memPool_WRITE_W_INITIALIZE && self->mode != memPool_WRITE_WO_INITIALIZE)
        return NULL;
        
    private_lfcMemPool_validate(self, "public_lfcMemPool_alloc begin");

    uint32_t real_needed_size = (needed_size + (uint32_t)(__BIGGEST_ALIGNMENT__ - 1)) &~ (__BIGGEST_ALIGNMENT__ - 1);

    if(0 == pthread_mutex_lock(&self->lock)) {
        // search a free header
        void *ptr = self->mem_ptr;

        memPool_headr_t *hdr;
        while (1) {
            hdr = (memPool_headr_t *)ptr;
            if (hdr->len == 0) {
                fprintf(stderr, "%s: Empty block found at address %p. This should not happen.\n", __FILE__, hdr);
            }
            if (LFCMEMPOOL_MAGIC != hdr->magic) {
                fprintf(
                    stderr, "%s@%d: Block w/ wrong magic pattern found: 0x%04x (0x%04x) (ptr=%p, self->mem_ptr=%p, self->usable_size=%zu, self->mem_ptr + self->usable_size=%p)\n", __func__, __LINE__,
                    hdr->magic, LFCMEMPOOL_MAGIC, ptr, self->mem_ptr, self->usable_size, self->mem_ptr + self->usable_size
                );

//                lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);

                pthread_mutex_unlock(&self->lock);
                return NULL;
            }

            if (hdr->isUsed == 0 && real_needed_size <= hdr->len) {
                if (private_lfcMemPool_updateHeader(self, hdr, real_needed_size)) {
                    pthread_mutex_unlock(&self->lock);
                    return NULL;
                }

                self->used_block_count++;
                self->used_size += (hdr->len + sizeof(memPool_headr_t));
                self->used_data_size += hdr->len;
                self->highwatermark_used_size = (self->highwatermark_used_size > self->used_size) ? self->highwatermark_used_size : self->used_size;

                private_lfcMemPool_validate(self, "public_lfcMemPool_alloc end");

                pthread_mutex_unlock(&self->lock);
                return hdr->data;
            } else {
                // not free or too small -> try next block
                ptr = ptr + hdr->len + sizeof(memPool_headr_t);

                // end of our mem?
                if (ptr >= (self->mem_ptr + self->usable_size)) {
                    pthread_mutex_unlock(&self->lock);
//                    fprintf(stderr, "Memory pool is full\n");
                    return NULL;
                }
            }
        }
    }

    return NULL;
}

/**
 * Diese Methode gibt einen freien Speicherblock zurueck und belegt ihn mit 0.
 */
static void *public_lfcMemPool_calloc(
    lfcMemPool_t *self,
    uint16_t needed_size
) {
    void *ret = public_lfcMemPool_alloc(self, needed_size);

    if (ret) {
        memset(ret, 0, needed_size);
    }

    return ret;
}

/**
 * Löscht die ptr aus der Liste.
 *
 * @param _self die Instanz der Liste selbst
 */
static int public_lfcMemPool_free(
    lfcMemPool_t *self,
    void *ptr
) {
    int result = -1;

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = private_lfcMemPool_free_nolock(self, ptr);

        pthread_mutex_unlock(&self->lock);
        private_lfcMemPool_validate(self, "public_lfcMemPool_free end");
    }

    return result;
}

/**
 * Defragmentieren den Scheiss.
 */
static int public_lfcMemPool_defrag (
    lfcMemPool_t *self
) {
    if(0 == pthread_mutex_lock(&self->lock)) {
        memPool_headr_t *current_hdr = self->mem_ptr;
        bool luecke = false;

        while (private_lfcMemPool_isOurMem(self, current_hdr)) {
            if (current_hdr->isUsed && luecke) {
                memPool_headr_t *prev_current_hdr = (memPool_headr_t *)((char *)current_hdr - current_hdr->prev_len - sizeof(memPool_headr_t));
                size_t complete_block_size = prev_current_hdr->len + sizeof(memPool_headr_t);
                memPool_headr_t *last_block = private_lfcMemPool_get_last_block(self);

                if (NULL == last_block) { return -EINVAL; } // ohaurha
                size_t last_block_len = last_block->len;

                // memmove von allem um luecke->len nach vorn
                size_t len_to_move = self->usable_size - ((char *)current_hdr - (char *)self->mem_ptr);
                memmove(prev_current_hdr, current_hdr, len_to_move);

                // hinten erstmal alles auf inval
                memPool_headr_t *new_hdr = self->mem_ptr + self->usable_size - complete_block_size;

                memset(new_hdr, LFCMEMPOOL_INVALID_MEM_PATTERN, complete_block_size);
//                lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);
                // neue letzten Header befüllen
                new_hdr->prev_len = last_block_len;
                new_hdr->magic = LFCMEMPOOL_MAGIC;
                new_hdr->isUsed = NOT_USED;
                new_hdr->len = complete_block_size - sizeof(memPool_headr_t);
//                lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);

                private_lfcMemPool_free_nolock(self, (char *)new_hdr + sizeof(memPool_headr_t)); // merge von evtl. daforliegenden leeren Bloecken
//                lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);

                luecke = false;
                current_hdr = (memPool_headr_t *)((char *)prev_current_hdr + prev_current_hdr->len + sizeof(memPool_headr_t));

                continue;
            } else if (current_hdr->isUsed && !luecke) {
                // einfach weiter machen
            } else {
                luecke = true;
            }

            current_hdr = (memPool_headr_t *)((char *)current_hdr + current_hdr->len + sizeof(memPool_headr_t));

//            lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);
        }

        pthread_mutex_unlock(&self->lock);
    }

    return 0;
}

/**
 * Gibt die Anzahl der genutzten Blöcke zurueck.
 */
static size_t public_lfcMemPool_getBlockCount(
    lfcMemPool_t *self
) {
    return self->used_block_count;
}

/**
 * Gibt die Block-Größe des Pointers zurück.
 *
 * @param _self die Instanz der Liste selbst
 */
static size_t public_lfcMemPool_getBlockSize(
    lfcMemPool_t *self,
    void *ptr
) {
    size_t result = 0;
    if (!ptr) {
        return 0;
    }
    if (!private_lfcMemPool_isOurMem(self, ptr)) {
        return 0;
    }

    if(0 == pthread_mutex_lock(&self->lock)) {
        memPool_headr_t *hdr = (memPool_headr_t *)(ptr - sizeof(memPool_headr_t));

        if (LFCMEMPOOL_MAGIC != hdr->magic) {
            fprintf(stderr, "%s@%d: Block w/ wrong magic pattern found: 0x%04x (0x%04x)\n", __func__, __LINE__, hdr->magic, LFCMEMPOOL_MAGIC);
//            lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);

            result = 0;
        } else {
            result = hdr->len;
        }

        pthread_mutex_unlock(&self->lock);
    }

    return result;
}

/**
 * Diese MEthode gibt die noch vorhandene nutzbare Anzahl an Bytes zurück.
 * Diese ist aber sehr wahrscheinlich fragmentiert.
 */
static size_t public_lfcMemPool_getFreeDataSize(
    lfcMemPool_t *self
) {
    return ((self->used_size + sizeof(memPool_headr_t)) > self->usable_size) ? 0 : (self->usable_size - self->used_size - sizeof(memPool_headr_t));
}

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
static int public_lfcMemPool_dump_va (
    lfcMemPool_t *self,
    size_t cols,
    char space,
    FILE *fp,
    const char *hdr_format,
    va_list hdr_format_args
) {
    if (!fp) { return -EINVAL; }

    char whole_line_pattern[cols];

    memset(whole_line_pattern, LFCMEMPOOL_INVALID_MEM_PATTERN, cols);

    if (hdr_format) {
        vfprintf(fp, hdr_format, hdr_format_args);
    }

    /*
     * Diese Funktion druckt alle folgenden Zeichen aus, bis es auf ein 0xaa trifft.
     * Wenn es auf das pattern trifft, füllt es noch die komplette Zeile auf.
     * D.h. es muss am Anfang einer Zeile auf ein Pattern treffen
     */
    size_t print_until_pattern (
        const char *ptr,
        size_t ptr_len
    ) {
        size_t iter;
        for (iter = 0; iter < ptr_len; iter++) {
            if ((iter % cols) == 0) {
                // besteht die nächste Zeile aus dem pattern?
                if ((ptr_len - iter >= cols) && (0 == memcmp(&ptr[iter], whole_line_pattern, cols))) {
                    return iter;
                }

                fprintf(fp, "\n");
            }

            fprintf(fp, "%02x%c", ptr[iter] & 0xff, space);
        }

        return iter;
    }

    /*
     * Diese Funktion druckt unser pattern aus, und zwar zählt es diese zusammen.
     */
    size_t print_pattern (
        const char *ptr,
        size_t ptr_len
    ) {
        size_t iter;

        fprintf(fp, "\n");
        for (iter = 0; iter < ptr_len; iter++) {
            if ((iter % cols) == 0) {
                // besteht die nächste Zeile aus dem pattern?
                if (0 != memcmp(&ptr[iter], whole_line_pattern, cols)) {
                    fprintf(fp, "    %zu x 0x%02x", iter, LFCMEMPOOL_INVALID_MEM_PATTERN);

                    return iter;
                }
            }
        }
        fprintf(fp, "    %zu x 0x%02x", iter, LFCMEMPOOL_INVALID_MEM_PATTERN);

        return iter;
    }

    // und jetzt mal alles raus
    size_t printed = 0;
    while (printed < self->usable_size) {
        printed += print_until_pattern(&((const char *)self->mem_ptr)[printed], self->usable_size - printed);
        if (printed >= self->usable_size) { break; }
        printed += print_pattern(&((const char *)self->mem_ptr)[printed], self->usable_size - printed);
    }

    fprintf(fp, "\n");

    return 0;
}

static uint8_t public_lfcMemPool_isValid(lfcMemPool_t *self, char **diagnostics) {
    int append_to_string(char **string, char *format, ...) {
        va_list varargs_first_call, varargs_second_call;
        va_start(varargs_first_call, format);
        va_copy(varargs_second_call, varargs_first_call);
        
        size_t length_of_data_to_append;
        int result = vsnprintf(NULL, 0, format, varargs_first_call);
        if (result < 0) {
            return result;
        }
        length_of_data_to_append = result;
        
        size_t length_of_old_string = strlen(*string);
        
        size_t length_of_new_string = length_of_old_string + length_of_data_to_append + 1;
        char *new_string = calloc(1, length_of_new_string);
        if (!new_string) {
            return -ENOMEM;
        }
        strncpy(new_string, *string, lfcMIN(length_of_new_string - 1, length_of_old_string));
        
        result = vsnprintf(new_string + length_of_old_string, length_of_new_string - length_of_old_string, format, varargs_second_call);
        if (result < 0) {
            return result;
        }
        
        free(*string);
        
        *string = new_string;
        
        va_end(varargs_second_call);
        va_end(varargs_first_call);
        
        return result;
    }
    
    if (!self) {
        return 0;
    }
    
    // initialize diagnostics to empty string
    *diagnostics = malloc(1);
    if (!*diagnostics) {
        return 0;
    }
    **diagnostics = 0;
    
        
    append_to_string(diagnostics, "Diagnostics for memory pool %p\n"\
                          "mem_ptr = %p\n"\
                          "usable_size = %d (memory from %p to %p)\n"\
                          "highwatermark_used_size = %d\n"\
                          "used_size = %d\n"\
                          "used_data_size = %d\n\n", self, 
                          self->mem_ptr, 
                          self->usable_size, self->mem_ptr, self->mem_ptr + self->usable_size - 1, 
                          self->highwatermark_used_size, 
                          self->used_size, 
                          self->used_data_size);
                                  
    void *current_block = self->mem_ptr;
    unsigned int prev_len = 0;
    uint8_t is_valid = 1;
    
    do {
        uint8_t current_block_is_invalid = 0;
        memPool_headr_t *current_header = (memPool_headr_t *) current_block;
        
        append_to_string(diagnostics, "Looking at block at %p\n"\
                                      "isUsed = %d\n"\
                                      "len = %d\n"\
                                      "prev_len = %d\n",
                                      current_header,
                                      current_header->isUsed,
                                      current_header->len,
                                      current_header->prev_len);
        
        if (current_block + sizeof(memPool_headr_t) + current_header->len > self->mem_ptr + self->usable_size) {
            append_to_string(diagnostics, "End of block is out of allowable memory\n");
            current_block_is_invalid = 1;
        }
        
        if (current_header->prev_len != prev_len) {
            append_to_string(diagnostics, "prev_len is invalid. Expected %d, but got %d\n", prev_len, current_header->prev_len);
            current_block_is_invalid = 1;
        }
        
        if (!current_block_is_invalid) {
            append_to_string(diagnostics, "Block looks good\n\n");
        } else {
            append_to_string(diagnostics, "BLOCK CORRUPTED\n\n");
            is_valid = 0;
        }
        
        prev_len = current_header->len;
        current_block = current_block + sizeof(memPool_headr_t) + current_header->len;
    } while (current_block < self->mem_ptr + self->usable_size);
    return is_valid;
}

/**
 * Gibt das nächste Element der Liste zurueck
 */
static void *public_lfcMemPool_next(
    lfcMemPool_t *self,
    const void *current
) {
    void *next_ptr = NULL;

    if (!private_lfcMemPool_isOurMem(self, current) && current != NULL) return NULL;

    if(0 == pthread_mutex_lock(&self->lock)) {
        // nächsten header suchen
        const memPool_headr_t *current_hdr = (memPool_headr_t *)(current - sizeof(memPool_headr_t));
        const memPool_headr_t *next_hdr = (current) ? (memPool_headr_t *)(((char *)current - sizeof(memPool_headr_t)) + current_hdr->len + sizeof(memPool_headr_t)) : self->mem_ptr;
        while (private_lfcMemPool_isOurMem(self, next_hdr) && !next_hdr->isUsed) {
            if (LFCMEMPOOL_MAGIC != next_hdr->magic) {
                fprintf(stderr, "%s@%d: Block w/ wrong magic pattern found: 0x%04x (0x%04x)\n", __func__, __LINE__, next_hdr->magic, LFCMEMPOOL_MAGIC);
//                lfcMemPool_dump(self, 32, ' ', stderr, "%d:%s", __LINE__, __func__);

                pthread_mutex_unlock(&self->lock);
                return NULL;
            }

            next_hdr = (const memPool_headr_t *)((char *)next_hdr + next_hdr->len + sizeof(memPool_headr_t));
        }
        next_ptr = (char *)next_hdr + sizeof(memPool_headr_t);

        pthread_mutex_unlock(&self->lock);
    }

    return private_lfcMemPool_isOurMem(self, next_ptr) ? next_ptr : NULL;
}

/**
 * Gibt das vorherige Element der Liste zurueck
 */
static void *public_lfcMemPool_prev(
    lfcMemPool_t *self,
    const void *current
) {
    void *iter = NULL;
    void *prev = NULL;

    while (NULL != (iter = public_lfcMemPool_next(self, iter))) {
        if (iter == current) { return prev; }

        prev = iter;
    }

    return NULL;
}

/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcMemPool)
        OVERRIDE_METHOD(lfcMemPool, alloc)
        OVERRIDE_METHOD(lfcMemPool, calloc)
        OVERRIDE_METHOD(lfcMemPool, free)

        OVERRIDE_METHOD(lfcMemPool, defrag)

        OVERRIDE_METHOD(lfcMemPool, getBlockCount)
        OVERRIDE_METHOD(lfcMemPool, getBlockSize)
        OVERRIDE_METHOD(lfcMemPool, getFreeDataSize)

        OVERRIDE_METHOD(lfcMemPool, next)
        OVERRIDE_METHOD(lfcMemPool, prev)

        OVERRIDE_METHOD(lfcMemPool, dump_va)
        OVERRIDE_METHOD(lfcMemPool, isValid)
    CLASS_CTOR__INIT_SUPER(lfcMemPool, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/
const lfcMemPool_class_t *lfcMemPool_class() {
    return _lfcMemPool_class
           ? _lfcMemPool_class
           : (_lfcMemPool_class = (lfcMemPool_class_t *) new(
            lfcObject_class(), "lfcMemPool_class", lfcObject_class(), sizeof(lfcMemPool_class_t),

            lfcObject_ctor, "", impl_lfcMemPool_class_ctor,

            (void *) 0)
           );
}

const lfcMemPool_t *lfcMemPool() {
    return _lfcMemPool
           ? _lfcMemPool
           : (_lfcMemPool = (lfcMemPool_t *) new(
            lfcMemPool_class(),
            "lfcMemPool", lfcObject(), sizeof(lfcMemPool_t),

            lfcObject_ctor, "ctor", public_lfcMemPool_ctor,
            lfcObject_dtor, "dtor", public_lfcMemPool_dtor,

            lfcMemPool_alloc, "alloc", public_lfcMemPool_alloc,
            lfcMemPool_calloc, "calloc", public_lfcMemPool_calloc,
            lfcMemPool_free, "free", public_lfcMemPool_free,

            lfcMemPool_defrag, "defrag", public_lfcMemPool_defrag,

            lfcMemPool_getBlockCount, "getBlockCount", public_lfcMemPool_getBlockCount,
            lfcMemPool_getBlockSize, "getBlockSize", public_lfcMemPool_getBlockSize,
            lfcMemPool_getFreeDataSize, "getFreeDataSize", public_lfcMemPool_getFreeDataSize,

            lfcMemPool_next, "next", public_lfcMemPool_next,
            lfcMemPool_prev, "prev", public_lfcMemPool_prev,

            lfcMemPool_dump_va, "dump_va", public_lfcMemPool_dump_va,
            lfcMemPool_isValid, "isValid", public_lfcMemPool_isValid,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcMemPool);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Liste mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcMemPool_t *lfcMemPool_ctor(
    char *mem_ptr,
    size_t usable_size,
    memPool_mode_e mode
) {
    return (lfcMemPool_t *)new(lfcMemPool(), mem_ptr, usable_size, mode);
}

int lfcMemPool_dump (
    lfcMemPool_t *self,
    size_t cols,
    char space,
    FILE *fp,
    const char *hdr_format,
    ...
) {
    int result;
    va_list args;

    va_start (args, hdr_format);
    result = lfcMemPool_dump_va(self, cols, space, fp, hdr_format, args);
    va_end (args);

    return result;
}

lfcOOP_accessor(lfcMemPool, alloc, void *, uint16_t)
lfcOOP_accessor(lfcMemPool, calloc, void *, uint16_t)
lfcOOP_accessor(lfcMemPool, free, int, void *)

lfcOOP_accessor(lfcMemPool, defrag, int)

lfcOOP_accessor(lfcMemPool, getBlockCount, size_t)
lfcOOP_accessor(lfcMemPool, getBlockSize, size_t, void *)
lfcOOP_accessor(lfcMemPool, getFreeDataSize, size_t)

lfcOOP_accessor(lfcMemPool, next, void *, const void *)
lfcOOP_accessor(lfcMemPool, prev, void *, const void *)

lfcOOP_accessor(lfcMemPool, dump_va, int, size_t, char, FILE *, const char *, va_list)
lfcOOP_accessor(lfcMemPool, isValid, uint8_t, char**)
