#ifndef LIBFORC_CORE_LFCENDIAN_H
#define LIBFORC_CORE_LFCENDIAN_H

/**
 * Die ganzen Klimpzüge mit einem eigenen endian-Header nur, weil
 * die Netzwerker eine eigene endian.h haben die dann natürlich von uns angezogen wird.
 */
#include <endian.h>

#ifndef htobe16
/* Conversion interfaces.  */
# include <byteswap.h>
#endif //#ifndef htobe16

# if __BYTE_ORDER == __LITTLE_ENDIAN

#ifndef htobe16
    #  define htobe16(x) __bswap_16 (x)
    #  define htole16(x) (x)
    #  define be16toh(x) __bswap_16 (x)
    #  define le16toh(x) (x)

    #  define htobe32(x) __bswap_32 (x)
    #  define htole32(x) (x)
    #  define be32toh(x) __bswap_32 (x)
    #  define le32toh(x) (x)

    #  define htobe64(x) __bswap_64 (x)
    #  define htole64(x) (x)
    #  define be64toh(x) __bswap_64 (x)
    #  define le64toh(x) (x)
#endif //#ifndef htobe16

# else

#ifndef htobe16
    #  define htobe16(x) (x)
    #  define htole16(x) __bswap_16 (x)
    #  define be16toh(x) (x)
    #  define le16toh(x) __bswap_16 (x)

    #  define htobe32(x) (x)
    #  define htole32(x) __bswap_32 (x)
    #  define be32toh(x) (x)
    #  define le32toh(x) __bswap_32 (x)

    #  define htobe64(x) (x)
    #  define htole64(x) __bswap_64 (x)
    #  define be64toh(x) (x)
    #  define le64toh(x) __bswap_64 (x)
#endif //#ifndef htobe16

# endif

#endif //LIBFORC_CORE_LFCENDIAN_H
