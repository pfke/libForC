#include "lfcByteBuffer.h"

#include <stdlib.h>
#include <asm/errno.h>
#include <core/lfcCore.h>

/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/
static const lfcByteBuffer_t *_lfcByteBuffer;
static const lfcByteBuffer_class_t *_lfcByteBuffer_class;
static lfcByteBuffer_methods_t _lfcByteBuffer_methods;


#define lfcBB_READ_DATA(size) \
    size_t result = public_lfcByteBuffer_read_int8ptr(self, sizeof(int##size##_t), (int8_t *)data);                     \
    if (data) {                                                                                                         \
        *data = (self->endian == lfcBB_BIG_ENDIAN)                                                                      \
            ? be##size##toh((uint##size##_t)*data)                                                                      \
            : le##size##toh((uint##size##_t)*data);                                                                     \
    }                                                                                                                   \
    return result;

#define lfcBB_WRITE_DATA(size) \
    int##size##_t toWrite = (self->endian == lfcBB_BIG_ENDIAN)                                                          \
        ? htobe##size((uint##size##_t)data)                                                                             \
        : htole##size((uint##size##_t)data);                                                                            \
    return public_lfcByteBuffer_write_int8ptr(self, sizeof(int##size##_t), (int8_t *)&toWrite);


/******************************************************************************************/
/* OBJECT METHODS                                                                         */
/******************************************************************************************/
static size_t   private_lfcByteBuffer_capacity_noLocking        (lfcByteBuffer_t *self);
static int      private_lfcByteBuffer_ensureWriteCapacity_noLocking(lfcByteBuffer_t *self, size_t size);
static int      private_lfcByteBuffer_ensureSize_noLocking      (lfcByteBuffer_t *self, size_t size);

static lfcByteBuffer_t            *public_lfcByteBuffer_ctor            (void *_self, va_list *app);
static lfcByteBuffer_t            *public_lfcByteBuffer_dtor            (lfcByteBuffer_t *self);

static size_t public_lfcByteBuffer_capacity      (lfcByteBuffer_t *self);
static int    public_lfcByteBuffer_isEmpty       (lfcByteBuffer_t *self);
static int    public_lfcByteBuffer_nonEmpty      (lfcByteBuffer_t *self);
static size_t public_lfcByteBuffer_read_int8     (lfcByteBuffer_t *self, int8_t *data);
static size_t public_lfcByteBuffer_read_int16    (lfcByteBuffer_t *self, int16_t *data);
static size_t public_lfcByteBuffer_read_int32    (lfcByteBuffer_t *self, int32_t *data);
static size_t public_lfcByteBuffer_read_int64    (lfcByteBuffer_t *self, int64_t *data);
static size_t public_lfcByteBuffer_read_uint8    (lfcByteBuffer_t *self, uint8_t *data);
static size_t public_lfcByteBuffer_read_uint16   (lfcByteBuffer_t *self, uint16_t *data);
static size_t public_lfcByteBuffer_read_uint32   (lfcByteBuffer_t *self, uint32_t *data);
static size_t public_lfcByteBuffer_read_uint64   (lfcByteBuffer_t *self, uint64_t *data);
static size_t public_lfcByteBuffer_read_int8ptr  (lfcByteBuffer_t *self, size_t max_buf_len, int8_t *buf);
static size_t public_lfcByteBuffer_write_int8    (lfcByteBuffer_t *self, int8_t data);
static size_t public_lfcByteBuffer_write_int16   (lfcByteBuffer_t *self, int16_t data);
static size_t public_lfcByteBuffer_write_int32   (lfcByteBuffer_t *self, int32_t data);
static size_t public_lfcByteBuffer_write_int64   (lfcByteBuffer_t *self, int64_t data);
static size_t public_lfcByteBuffer_write_uint8   (lfcByteBuffer_t *self, uint8_t data);
static size_t public_lfcByteBuffer_write_uint16  (lfcByteBuffer_t *self, uint16_t data);
static size_t public_lfcByteBuffer_write_uint32  (lfcByteBuffer_t *self, uint32_t data);
static size_t public_lfcByteBuffer_write_uint64  (lfcByteBuffer_t *self, uint64_t data);
static size_t public_lfcByteBuffer_write_int8ptr (lfcByteBuffer_t *self, size_t len, const int8_t *buf);


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

/**
 * Diese Methode gibt die Anzahl der noch zu lesenden Bytes zurueck.
 *
 * @param self ist der Instanz-Pointer
 * @return Anzahl der noch zu lesenden Bytes
 */
static size_t private_lfcByteBuffer_capacity_noLocking (
    lfcByteBuffer_t *self
) {
    return self->write_ptr - self->read_ptr;
}

/**
 * Diese Methode stellt sicher, dass min. die angegebene Anzahlk an Bytes noch reinpasst.
 *
 * @param self ist der Instanz-Pointer
 * @param size ist die min. freie Größe
 * @return 0 .. alles ok
 *         <0 .. Fehler
 */
static int private_lfcByteBuffer_ensureWriteCapacity_noLocking(
    lfcByteBuffer_t *self,
    size_t size
) {
    size_t needed_capacity = size - private_lfcByteBuffer_capacity_noLocking(self);

    if (needed_capacity == 0) {
        return 0;
    } else if (self->read_ptr >= needed_capacity) {
        // mov mem
        memmove(self->table, &self->table[self->read_ptr], self->write_ptr - self->read_ptr);
        self->write_ptr -= self->read_ptr;
        self->read_ptr = 0;

        return 0;
    } else {
        return private_lfcByteBuffer_ensureSize_noLocking(self, self->table_size + (size - private_lfcByteBuffer_capacity_noLocking(self)));
    }
}

/**
 * Diese Methode stellt sicher, dass die interne Tabelle so groß ist wie angegben.
 *
 * @param self ist der Instanz-Pointer
 * @param size ist die erwartete min. Größe der Tabelle
 * @return 0 .. alles ok
 *         <0 .. Fehler
 */
static int private_lfcByteBuffer_ensureSize_noLocking (
    lfcByteBuffer_t *self,
    size_t size
) {
    int8_t *new_table;

    if (self->table_size >= size) {
        return 0;
    }

    new_table = realloc(self->table, size);

    if (!new_table) {
        return -ENOMEM;
    }

    self->table = new_table;
    self->table_size = size;

    return 0;
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
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
static lfcByteBuffer_t *public_lfcByteBuffer_ctor(
    void *_self,
    va_list *app
) {
    lfcByteBuffer_t *self = (lfcByteBuffer_t *) lfcObject_super_ctor(lfcByteBuffer(), _self);

    if (self == 0)
        return 0;

    // read args
    self->endian = va_arg(*app, lfcByteBuffer_endian_e);
    self->table_size = va_arg(*app, size_t);
    self->increaseBy = va_arg(*app, size_t);
    self->read_ptr = 0;
    self->write_ptr = 0;

    self->table = (int8_t *)calloc(self->table_size, sizeof(int8_t));
    if (!self->table) {
        goto err_no_mem;
    }

    pthread_mutex_init(&self->lock, NULL);

    return self;

err_no_mem:
    delete(self);

    return NULL;
}

/**
 * Destructor der Liste selbst.
 */
static lfcByteBuffer_t *public_lfcByteBuffer_dtor(
    lfcByteBuffer_t *self
) {
    lfcObject_super_dtor(lfcByteBuffer(), self);

    free(self->table);
    self->table = 0;
    self->table_size = 0;
    self->read_ptr = 0;
    self->write_ptr = 0;

    pthread_mutex_destroy(&self->lock);

    return self;
}

/**
 * Diese Methode gibt die Anzahl der noch zu lesenden Bytes zurueck.
 *
 * @param self ist der Instanz-Pointer
 * @return Anzahl der noch zu lesenden Bytes
 */
size_t public_lfcByteBuffer_capacity (
    lfcByteBuffer_t *self
) {
    size_t result = 0;

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = private_lfcByteBuffer_capacity_noLocking(self);

        pthread_mutex_unlock(&self->lock);
    }

    return result;
}

/**
 * Methode um abzufragen, ob der buffer leer ist.
 *
 * @param self ist der Instanz-Pointer
 * @return 1 .. nix mehr zu lesen
 *         0 .. es kann noch gelesen werden
 */
int public_lfcByteBuffer_isEmpty (
    lfcByteBuffer_t *self
) {
    int result = 1;

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = self->write_ptr == self->read_ptr;

        pthread_mutex_unlock(&self->lock);
    }

    return result;
}

/**
 * Methode um abzufragen, ob man noch was lesen kann.
 *
 * @param self ist der Instanz-Pointer
 * @return 0 .. nix mehr zu lesen
 *         1 .. es kann noch gelesen werden
 */
int public_lfcByteBuffer_nonEmpty (
    lfcByteBuffer_t *self
) {
    return !public_lfcByteBuffer_isEmpty(self);
}

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
static size_t public_lfcByteBuffer_read_int8   (lfcByteBuffer_t *self, int8_t *data)   { return public_lfcByteBuffer_read_int8ptr(self, sizeof(int8_t), data); }
static size_t public_lfcByteBuffer_read_int16  (lfcByteBuffer_t *self, int16_t *data)  { lfcBB_READ_DATA(16); }
static size_t public_lfcByteBuffer_read_int32  (lfcByteBuffer_t *self, int32_t *data)  { lfcBB_READ_DATA(32); }
static size_t public_lfcByteBuffer_read_int64  (lfcByteBuffer_t *self, int64_t *data)  { lfcBB_READ_DATA(64); }
static size_t public_lfcByteBuffer_read_uint8  (lfcByteBuffer_t *self, uint8_t *data)  { return public_lfcByteBuffer_read_int8ptr(self, sizeof(uint8_t), (int8_t *)data); }
static size_t public_lfcByteBuffer_read_uint16 (lfcByteBuffer_t *self, uint16_t *data) { lfcBB_READ_DATA(16) }
static size_t public_lfcByteBuffer_read_uint32 (lfcByteBuffer_t *self, uint32_t *data) { lfcBB_READ_DATA(32); }
static size_t public_lfcByteBuffer_read_uint64 (lfcByteBuffer_t *self, uint64_t *data) { lfcBB_READ_DATA(64); }

/**
 * Diese Methode liest aus dem Bytebuffer eine Anzahl Bytes (je nach Datentyp).
 * Es wird von VORN gelesen, bis nichts mehr drin ist.
 *
 * @param self ist der Instanz-Pointer
 * @param max_buf_len ist die maximale buffer länge
 * @param buf ist der buffer in den hineingelesen werden soll
 * @return Anzahl der gelesenen Bytes
 */
static size_t public_lfcByteBuffer_read_int8ptr (
    lfcByteBuffer_t *self,
    size_t max_buf_len,
    int8_t *buf
) {
    if (max_buf_len == 0) {
        return 0;
    }

    if(0 == pthread_mutex_lock(&self->lock)) {
        if (private_lfcByteBuffer_capacity_noLocking(self) < max_buf_len) {
            return 0;
        }

        if (buf) {
            memcpy(buf, &self->table[self->read_ptr], max_buf_len);
        }
        self->read_ptr += max_buf_len;

        pthread_mutex_unlock(&self->lock);

        return max_buf_len;
    }

    return 0;
}

/**
 * Diese Methode schreibt das gegebene Datum in den ByteString, und zwar hinten dran!
 *
 * @param self ist der Instanz-Pointer
 * @param data ist das zu schreibende Datum
 * @return Anzahl der geschriebenen Bytes
 */
static size_t public_lfcByteBuffer_write_int8   ( lfcByteBuffer_t *self, int8_t data )   { return public_lfcByteBuffer_write_int8ptr(self, sizeof(int8_t), &data); }
static size_t public_lfcByteBuffer_write_int16  ( lfcByteBuffer_t *self, int16_t data )  { lfcBB_WRITE_DATA(16); }
static size_t public_lfcByteBuffer_write_int32  ( lfcByteBuffer_t *self, int32_t data )  { lfcBB_WRITE_DATA(32); }
static size_t public_lfcByteBuffer_write_int64  ( lfcByteBuffer_t *self, int64_t data )  { lfcBB_WRITE_DATA(64); }
static size_t public_lfcByteBuffer_write_uint8  ( lfcByteBuffer_t *self, uint8_t data )  { return public_lfcByteBuffer_write_int8ptr(self, sizeof(uint8_t), (int8_t *)&data); }
static size_t public_lfcByteBuffer_write_uint16 ( lfcByteBuffer_t *self, uint16_t data ) { lfcBB_WRITE_DATA(16); }
static size_t public_lfcByteBuffer_write_uint32 ( lfcByteBuffer_t *self, uint32_t data ) { lfcBB_WRITE_DATA(32); }
static size_t public_lfcByteBuffer_write_uint64 ( lfcByteBuffer_t *self, uint64_t data ) { lfcBB_WRITE_DATA(64); }

/**
 * Diese Methode schreibt das gegebene Datum in den ByteString, und zwar hinten dran!
 *
 * @param self ist der Instanz-Pointer
 * @param len ist die zu schreibende Länge
 * @param buf ist der Buffer mit dem zu schreibendem Inhalt
 * @return Anzahl der geschriebenen Bytes
 */
static size_t public_lfcByteBuffer_write_int8ptr (
    lfcByteBuffer_t *self,
    size_t len,
    const int8_t *buf
) {
    int result = 0;

    if (len == 0) {
        return 0;
    }
    if (!buf) {
        return 0;
    }

    if(0 == pthread_mutex_lock(&self->lock)) {
        result = private_lfcByteBuffer_ensureWriteCapacity_noLocking(self, len);

        if (result) {
            pthread_mutex_unlock(&self->lock);
            return 0;
        }

        memcpy(&self->table[self->write_ptr], buf, len);
        self->write_ptr += len;

        pthread_mutex_unlock(&self->lock);
        return len;
    }

    return 0;
}

/**
 * Gibt ein geclontes Array von read-ptr bis write-ptr zurück.
 *
 * @param self
 * @return Buffer der selbst freigegeben werden muss.
 */
static size_t public_lfcByteBuffer_toArray (
    lfcByteBuffer_t *self,
    int8_t **result
) {
    if (!result) { return 0; }

    size_t array_size = self->write_ptr - self->read_ptr;
    int8_t *result_ptr = malloc(lfcMAX(0, self->write_ptr - self->read_ptr));
    if (!result_ptr) { return 0; }

    *result = result_ptr;

    memcpy(result_ptr, &self->table[self->read_ptr], array_size);

    return array_size;
}


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

/**
 * Ctor der Klasse.
 * Setzt u.a. die Methoden.
 *
 * @param _self die Instanz selbst
 * @param app Argumentenliste (Methoden, Name, Größe, ...)
 *
 * @return die Instanz selbst
 */
CLASS_CTOR__START(lfcByteBuffer)
        OVERRIDE_METHOD(lfcByteBuffer, capacity)
        OVERRIDE_METHOD(lfcByteBuffer, isEmpty)
        OVERRIDE_METHOD(lfcByteBuffer, nonEmpty)

        OVERRIDE_METHOD(lfcByteBuffer, read_int8)
        OVERRIDE_METHOD(lfcByteBuffer, read_int16)
        OVERRIDE_METHOD(lfcByteBuffer, read_int32)
        OVERRIDE_METHOD(lfcByteBuffer, read_int64)
        OVERRIDE_METHOD(lfcByteBuffer, read_uint8)
        OVERRIDE_METHOD(lfcByteBuffer, read_uint16)
        OVERRIDE_METHOD(lfcByteBuffer, read_uint32)
        OVERRIDE_METHOD(lfcByteBuffer, read_uint64)
        OVERRIDE_METHOD(lfcByteBuffer, read_int8ptr)

        OVERRIDE_METHOD(lfcByteBuffer, write_int8)
        OVERRIDE_METHOD(lfcByteBuffer, write_int16)
        OVERRIDE_METHOD(lfcByteBuffer, write_int32)
        OVERRIDE_METHOD(lfcByteBuffer, write_int64)
        OVERRIDE_METHOD(lfcByteBuffer, write_uint8)
        OVERRIDE_METHOD(lfcByteBuffer, write_uint16)
        OVERRIDE_METHOD(lfcByteBuffer, write_uint32)
        OVERRIDE_METHOD(lfcByteBuffer, write_uint64)
        OVERRIDE_METHOD(lfcByteBuffer, write_int8ptr)

        OVERRIDE_METHOD(lfcByteBuffer, toArray)
    CLASS_CTOR__INIT_SUPER(lfcByteBuffer, lfcObject)
    CLASS_CTOR__INIT_IFACES()
CLASS_CTOR__END()

const lfcByteBuffer_class_t *lfcByteBuffer_class() {
    return _lfcByteBuffer_class
           ? _lfcByteBuffer_class
           : (_lfcByteBuffer_class = (lfcByteBuffer_class_t *) new(
            lfcObject_class(), "lfcByteBuffer_class", lfcObject_class(), sizeof(lfcByteBuffer_class_t),

            lfcObject_ctor, "", impl_lfcByteBuffer_class_ctor,

            (void *) 0)
           );
}

const lfcByteBuffer_t *lfcByteBuffer() {
    return _lfcByteBuffer
           ? _lfcByteBuffer
           : (_lfcByteBuffer = (lfcByteBuffer_t *) new(
            lfcByteBuffer_class(),
            "lfcByteBuffer", lfcObject(), sizeof(lfcByteBuffer_t),

            lfcObject_ctor, "ctor", public_lfcByteBuffer_ctor,
            lfcObject_dtor, "dtor", public_lfcByteBuffer_dtor,
            lfcByteBuffer_capacity, "capacity", public_lfcByteBuffer_capacity,
            lfcByteBuffer_isEmpty, "isEmpty", public_lfcByteBuffer_isEmpty,
            lfcByteBuffer_nonEmpty, "nonEmpty", public_lfcByteBuffer_nonEmpty,

            lfcByteBuffer_read_int8, "read_int8", public_lfcByteBuffer_read_int8,
            lfcByteBuffer_read_int16, "read_int16", public_lfcByteBuffer_read_int16,
            lfcByteBuffer_read_int32, "read_int32", public_lfcByteBuffer_read_int32,
            lfcByteBuffer_read_int64, "read_int64", public_lfcByteBuffer_read_int64,
            lfcByteBuffer_read_uint8, "read_uint8", public_lfcByteBuffer_read_uint8,
            lfcByteBuffer_read_uint16, "read_uint16", public_lfcByteBuffer_read_uint16,
            lfcByteBuffer_read_uint32, "read_uint32", public_lfcByteBuffer_read_uint32,
            lfcByteBuffer_read_uint64, "read_uint64", public_lfcByteBuffer_read_uint64,
            lfcByteBuffer_read_int8ptr, "read_int8ptr", public_lfcByteBuffer_read_int8ptr,

            lfcByteBuffer_write_int8, "write_int8", public_lfcByteBuffer_write_int8,
            lfcByteBuffer_write_int16, "write_int16", public_lfcByteBuffer_write_int16,
            lfcByteBuffer_write_int32, "write_int32", public_lfcByteBuffer_write_int32,
            lfcByteBuffer_write_int64, "write_int64", public_lfcByteBuffer_write_int64,
            lfcByteBuffer_write_uint8, "write_uint8", public_lfcByteBuffer_write_uint8,
            lfcByteBuffer_write_uint16, "write_uint16", public_lfcByteBuffer_write_uint16,
            lfcByteBuffer_write_uint32, "write_uint32", public_lfcByteBuffer_write_uint32,
            lfcByteBuffer_write_uint64, "write_uint64", public_lfcByteBuffer_write_uint64,
            lfcByteBuffer_write_int8ptr, "write_int8ptr", public_lfcByteBuffer_write_int8ptr,

            lfcByteBuffer_toArray, "toArray", public_lfcByteBuffer_toArray,

            (void *) 0)
           );
}

CLASS_MAKE_METHODS_FUNC(lfcByteBuffer);

/******************************************************************************************/
/* ACCESSOR METHODS                                                                       */
/******************************************************************************************/

/**
 * Liste mit default-Größen initialisieren.
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcByteBuffer_t *lfcByteBuffer_ctor() {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    lfcByteBuffer_endian_e endian_to_use = lfcBB_LITTLE_ENDIAN;
#elif __BYTE_ORDER == __BIG_ENDIAN
    lfcByteBuffer_endian_e endian_to_use = lfcBB_BIG_ENDIAN;
#else
    #error compiles only on big or little endian systems
#endif

    return lfcByteBuffer_ctorComplete(endian_to_use, CBB_DEFAULT_INITIAL_SIZE, CBB_DEFAULT_INCREASE_BY);
}

/**
 * Liste mit den angegebenen Größen initialisieren
 *
 * @param endian so wird aus dem Buffer gelesen und geschrieben
 * @param initial_size ist die initiale Größe der Liste
 * @param increase_by um diesen Wert wird die Liste bei Bedarf vergrößert
 *
 * @return eine Objekt-Instanz der Liste
 */
lfcByteBuffer_t *lfcByteBuffer_ctorComplete(
    lfcByteBuffer_endian_e endian,
    size_t initial_size,
    size_t increase_by
) {
    return (lfcByteBuffer_t *)new(lfcByteBuffer(), endian, initial_size, increase_by);
}

lfcByteBuffer_t *lfcByteBuffer_init_fromArray(
    lfcByteBuffer_endian_e endian,
    size_t len,
    int8_t *initial_array
) {
    lfcByteBuffer_t *bb = lfcByteBuffer_ctorComplete(endian, len, CBB_DEFAULT_INCREASE_BY);

    if (bb) {
        if (lfcByteBuffer_write_int8ptr(bb, len, initial_array) != len) {
            delete(bb);
            bb = NULL;
        }
    }

    return bb;
}

lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, capacity, size_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, isEmpty, int)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, nonEmpty, int)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_int8, size_t, int8_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_int16, size_t, int16_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_int32, size_t, int32_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_int64, size_t, int64_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_uint8, size_t, uint8_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_uint16, size_t, uint16_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_uint32, size_t, uint32_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_uint64, size_t, uint64_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, read_int8ptr, size_t, size_t, int8_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_int8, size_t, int8_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_int16, size_t, int16_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_int32, size_t, int32_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_int64, size_t, int64_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_uint8, size_t, uint8_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_uint16, size_t, uint16_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_uint32, size_t, uint32_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_uint64, size_t, uint64_t)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, write_int8ptr, size_t, size_t, const int8_t *)
lfcOOP_IMPL_ACCESSOR(lfcByteBuffer, toArray, size_t, int8_t **)
