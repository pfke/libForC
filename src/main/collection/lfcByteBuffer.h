#ifndef LIBFORC_COLLECTION_LFCBYTEBUFFER_H
#define LIBFORC_COLLECTION_LFCBYTEBUFFER_H

    #include <stdint.h>
    #include <pthread.h>
    #include "../core/lfcObject.h"

    #define CBB_DEFAULT_INITIAL_SIZE            100
    #define CBB_DEFAULT_INCREASE_BY              10

    typedef enum lfcByteBuffer_endian lfcByteBuffer_endian_e;
    enum lfcByteBuffer_endian {
        lfcBB_BIG_ENDIAN,
        lfcBB_LITTLE_ENDIAN,
    };

    DEFINE_CLASS(lfcByteBuffer)

    struct lfcByteBuffer { const struct lfcObject _;
        lfcByteBuffer_endian_e endian;

        int8_t *table;
        size_t table_size;
        size_t increaseBy;

        size_t read_ptr;
        size_t write_ptr;

        pthread_mutex_t lock;
    };

    struct lfcByteBuffer_class { const struct lfcObject_class _;
      method_t capacity;
      method_t isEmpty;
      method_t nonEmpty;

      method_t read_int8;
      method_t read_int16;
      method_t read_int32;
      method_t read_int64;
      method_t read_int8ptr;
      method_t read_uint8;
      method_t read_uint16;
      method_t read_uint32;
      method_t read_uint64;

      method_t write_int8;
      method_t write_int16;
      method_t write_int32;
      method_t write_int64;
      method_t write_int8ptr;
      method_t write_uint8;
      method_t write_uint16;
      method_t write_uint32;
      method_t write_uint64;
    };

    struct lfcByteBuffer_methods {
        size_t (*capacity)(lfcByteBuffer_t *self);
        int (*isEmpty)(lfcByteBuffer_t *self);
        int (*nonEmpty)(lfcByteBuffer_t *self);

        size_t (*read_int8)     (lfcByteBuffer_t *self, int8_t *data);
        size_t (*read_int16)    (lfcByteBuffer_t *self, int16_t *data);
        size_t (*read_int32)    (lfcByteBuffer_t *self, int32_t *data);
        size_t (*read_int64)    (lfcByteBuffer_t *self, int64_t *data);
        size_t (*read_uint8)    (lfcByteBuffer_t *self, uint8_t *data);
        size_t (*read_uint16)   (lfcByteBuffer_t *self, uint16_t *data);
        size_t (*read_uint32)   (lfcByteBuffer_t *self, uint32_t *data);
        size_t (*read_uint64)   (lfcByteBuffer_t *self, uint64_t *data);
        size_t (*read_int8ptr)  (lfcByteBuffer_t *self, size_t max_buf_len, int8_t *buf);

        size_t (*write_int8)    (lfcByteBuffer_t *self, int8_t data);
        size_t (*write_int16)   (lfcByteBuffer_t *self, int16_t data);
        size_t (*write_int32)   (lfcByteBuffer_t *self, int32_t data);
        size_t (*write_int64)   (lfcByteBuffer_t *self, int64_t data);
        size_t (*write_uint8)   (lfcByteBuffer_t *self, uint8_t data);
        size_t (*write_uint16)  (lfcByteBuffer_t *self, uint16_t data);
        size_t (*write_uint32)  (lfcByteBuffer_t *self, uint32_t data);
        size_t (*write_uint64)  (lfcByteBuffer_t *self, uint64_t data);
        size_t (*write_int8ptr) (lfcByteBuffer_t *self, size_t len, int8_t *buf);

        // super
        const lfcObject_methods_t *base;
    };

    lfcByteBuffer_t *lfcByteBuffer_ctor(void);
    lfcByteBuffer_t *lfcByteBuffer_ctorComplete(
        lfcByteBuffer_endian_e endian,
        size_t initial_size,
        size_t increase_by
    );

    lfcByteBuffer_t *lfcByteBuffer_init_wArray(
        lfcByteBuffer_endian_e endian,
        size_t len,
        int8_t *initial_array
    );

    /**
     * Diese Methode gibt die Anzahl der noch zu lesenden Bytes zurueck.
     *
     * @param self ist der Instanz-Pointer
     * @return Anzahl der noch zu lesenden Bytes
     */
    size_t lfcByteBuffer_capacity(lfcByteBuffer_t *self);

    /**
     * Methode um abzufragen, ob der buffer leer ist.
     *
     * @param self ist der Instanz-Pointer
     * @return 1 .. nix mehr zu lesen
     *         0 .. es kann noch gelesen werden
     */
    int lfcByteBuffer_isEmpty(lfcByteBuffer_t *self);

    /**
     * Methode um abzufragen, ob man noch was lesen kann.
     *
     * @param self ist der Instanz-Pointer
     * @return 0 .. nix mehr zu lesen
     *         1 .. es kann noch gelesen werden
     */
    int lfcByteBuffer_nonEmpty(lfcByteBuffer_t *self);

    /**
     * Diese Methode liest aus dem Bytebuffer eine Anzahl Bytes (je nach Datentyp).
     * Es wird von VORN gelesen, bis nichts mehr drin ist.
     *
     * Achtung: !!! Nicht multithreading fest. !!!
     *
     * @param self ist der Instanz-Pointer
     * @param data hier wird der gelesene Wert raufgeschrieben
     * @return Anzahl der gelesenen Bytes
     */
    size_t lfcByteBuffer_read_int8  (lfcByteBuffer_t *self, int8_t *data);
    size_t lfcByteBuffer_read_int16 (lfcByteBuffer_t *self, int16_t *data);
    size_t lfcByteBuffer_read_int32 (lfcByteBuffer_t *self, int32_t *data);
    size_t lfcByteBuffer_read_int64 (lfcByteBuffer_t *self, int64_t *data);
    size_t lfcByteBuffer_read_uint8 (lfcByteBuffer_t *self, uint8_t *data);
    size_t lfcByteBuffer_read_uint16(lfcByteBuffer_t *self, uint16_t *data);
    size_t lfcByteBuffer_read_uint32(lfcByteBuffer_t *self, uint32_t *data);
    size_t lfcByteBuffer_read_uint64(lfcByteBuffer_t *self, uint64_t *data);

    /**
     * Diese Methode liest aus dem Bytebuffer eine Anzahl Bytes (je nach Datentyp).
     * Es wird von VORN gelesen, bis nichts mehr drin ist.
     *
     * @param self ist der Instanz-Pointer
     * @param max_buf_len ist die maximale buffer länge
     * @param buf ist der buffer in den hineingelesen werden soll
     * @return Anzahl der gelesenen Bytes
     */
    size_t lfcByteBuffer_read_int8ptr(lfcByteBuffer_t *self, size_t max_buf_len, int8_t *buf);

    /**
     * Diese Methode schreibt das gegebene Datum in den ByteString, und zwar hinten dran!
     *
     * @param self ist der Instanz-Pointer
     * @param data ist das zu schreibende Datum
     * @return Anzahl der geschriebenen Bytes
     */
    size_t lfcByteBuffer_write_int8   (lfcByteBuffer_t *self, int8_t data);
    size_t lfcByteBuffer_write_int16  (lfcByteBuffer_t *self, int16_t data);
    size_t lfcByteBuffer_write_int32  (lfcByteBuffer_t *self, int32_t data);
    size_t lfcByteBuffer_write_int64  (lfcByteBuffer_t *self, int64_t data);
    size_t lfcByteBuffer_write_uint8  (lfcByteBuffer_t *self, uint8_t data);
    size_t lfcByteBuffer_write_uint16 (lfcByteBuffer_t *self, uint16_t data);
    size_t lfcByteBuffer_write_uint32 (lfcByteBuffer_t *self, uint32_t data);
    size_t lfcByteBuffer_write_uint64 (lfcByteBuffer_t *self, uint64_t data);

    /**
     * Diese Methode schreibt das gegebene Datum in den ByteString, und zwar hinten dran!
     *
     * @param self ist der Instanz-Pointer
     * @param len ist die zu schreibende Länge
     * @param buf ist der Buffer mit dem zu schreibendem Inhalt
     * @return Anzahl der geschriebenen Bytes
     */
    size_t lfcByteBuffer_write_int8ptr(lfcByteBuffer_t *self, size_t len, int8_t *buf);


#endif //LIBFORC_COLLECTION_LFCBYTEBUFFER_H
