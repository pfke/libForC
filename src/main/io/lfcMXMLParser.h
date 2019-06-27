#ifndef LIBFORC_IO_MXMLPARSER_H_
#define LIBFORC_IO_MXMLPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "../collection/lfcList.h"
#include "../tools/logging/lfcLogger.h"
#include <mxml.h>


#define lfcMXML_LOGGER_PREFIX       "mxml-parser"

typedef enum lfcMXML_argType {
    LFCMXML_ARGTYPE_XMLSTRING = 1,
    LFCMXML_ARGTYPE_XMLFILE,
    LFCMXML_ARGTYPE_LOGGER,
    LFCMXML_ARGTYPE_MAINNODE_NAME,
    LFCMXML_ARGTYPE_MAINNODE_CALLBACK,
} lfcMXML_argType_e;

typedef void *(*lfcMXML_parseMainNode_fn)(lfcLogger_t *, mxml_node_t *);

/**
 * Parse irgendwas.
 * Alle Argumente werden als Tuple
 *   lfcMXML_argType_e -> value
 * übergeben.
 *
 * !!! Das letzte Argument MUSS NULL sein !!!
 */
lfcList_t *lfcMXMLParser_parse(
    lfcMXML_argType_e argType,
    ...
);


#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_IO_MXMLPARSER_H_ ---*/
