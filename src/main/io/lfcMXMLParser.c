#include "lfcMXMLParser.h"

#include "collection/lfcHashmap.h"

#include <errno.h>
#include <mxml.h>
#include <stdarg.h>


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/


/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/

lfcList_t *private_lfcMXMLParser_parse_xml(
    lfcLogger_t *logger,
    mxml_node_t *rootNode,
    const char *mainNode,
    lfcMXML_parseMainNode_fn parseMainNode_cb
) {
    lfcLogger_INFO(logger, "start parsing xml...");

    // arg checks
    if (!rootNode) {
        lfcLogger_ERR(logger, "no xml given");
        return NULL;
    }
    if (!parseMainNode_cb) {
        lfcLogger_ERR(logger, "parseMainNode callback not given");
        return NULL;
    }
    if (!mainNode) {
        lfcLogger_INFO(logger, "no main node given. Call callback for all child node...");
    }

    // los gehts
    lfcList_t *result = lfcList_ctor();
    if (!result) {
        lfcLogger_ERR(logger, "could not create result list");
        return NULL;
    }

    mxml_node_t *child;
    for (
        child = mxmlWalkNext(rootNode, rootNode, MXML_DESCEND_FIRST);
        NULL != child;
        child = mxmlWalkNext(child, rootNode, MXML_NO_DESCEND)
        ) {
        if (child->type != MXML_ELEMENT) { continue; }

        if (mainNode) {
            const char *nodeName = child->value.element.name;
            if (strlen(nodeName) != strlen(mainNode) && 0 != strncmp(nodeName, mainNode, strlen(mainNode))) {
                lfcLogger_DEBUG(logger, "ignore node: '%s'", child->value.element.name);
                continue;
            }
        }

        lfcLogger_DEBUG(logger, "found node: '%s', calling %p", child->value.element.name, parseMainNode_cb);
        lfcList_add(result, parseMainNode_cb(logger, child));
    }

    mxmlDelete(rootNode);

    lfcLogger_INFO(logger, "finished parsing xml");

    return result;
}

lfcList_t *private_lfcMXMLParser_parse_xmlFile(
    lfcLogger_t *logger,
    const char *xmlFile,
    const char *mainNode,
    lfcMXML_parseMainNode_fn parseMainNode_cb
) {
    // arg checks
    if (!xmlFile) {
        lfcLogger_ERR(logger, "no xml file given");
        return NULL;
    }
    if (!parseMainNode_cb) {
        lfcLogger_ERR(logger, "parseMainNode callback not given");
        return NULL;
    }
    if (!mainNode) {
        lfcLogger_INFO(logger, "no main node given. Call callback for all child node...");
    }

    lfcLogger_INFO(logger, "start parsing xml from file '%s'", xmlFile);

    // los gehts
    FILE *fd = fopen(xmlFile, "r");

    lfcList_t *result;
    if (fd) {
        result = private_lfcMXMLParser_parse_xml(
            logger,
            mxmlLoadFile(NULL, fd, MXML_NO_CALLBACK),
            mainNode,
            parseMainNode_cb
        );

        fclose(fd);
    } else {
        result = lfcList_ctor();
    }

    return result;
}

lfcList_t *private_lfcMXMLParser_parse_xmlString(
    lfcLogger_t *logger,
    const char *xmlString,
    const char *mainNode,
    lfcMXML_parseMainNode_fn parseMainNode_cb
) {
    lfcLogger_INFO(logger, "start parsing xml...");

    // arg checks
    if (!xmlString) {
        lfcLogger_ERR(logger, "no xml given");
        return NULL;
    }
    if (!parseMainNode_cb) {
        lfcLogger_ERR(logger, "parseMainNode callback not given");
        return NULL;
    }
    if (!mainNode) {
        lfcLogger_INFO(logger, "no main node given. Call callback for all child node...");
    }

    // los gehts
    return private_lfcMXMLParser_parse_xml(
        logger,
        mxmlLoadString(NULL, xmlString, MXML_NO_CALLBACK),
        mainNode,
        parseMainNode_cb
    );
}


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Parse irgendwas.
 * Alle Argumente werden als Tuple
 *   lfcMXML_argType_e -> value
 * übergeben.
 */
lfcList_t *lfcMXMLParser_parse(
    lfcMXML_argType_e argType,
    ...
) {
    va_list ap;
    lfcHashmap_t *args = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcList_t *result = NULL;
    lfcLogger_t *logger = NULL;

    if (!args) {
        lfcLogger_ERR_g("[lfcMXML] could not create arg hashmap");
        return NULL;
    }

    // read arguments
    {
        va_start(ap, argType);

        lfcMXML_argType_e currentArgType = argType;
        while (currentArgType != 0) {
            switch (currentArgType) {
                case LFCMXML_ARGTYPE_LOGGER:
                    lfcHashmap_put(args, (intptr_t *)LFCMXML_ARGTYPE_LOGGER, va_arg(ap, lfcLogger_t *));
                    break;

                case LFCMXML_ARGTYPE_MAINNODE_CALLBACK:
                    lfcHashmap_put(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_CALLBACK, va_arg(ap, lfcMXML_parseMainNode_fn));
                    break;
                case LFCMXML_ARGTYPE_MAINNODE_NAME:
                    lfcHashmap_put(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_NAME, (char *) va_arg(ap, const char *));
                    break;

                case LFCMXML_ARGTYPE_XMLFILE:
                    lfcHashmap_put(args, (intptr_t *)LFCMXML_ARGTYPE_XMLFILE, (char *) va_arg(ap, const char *));
                    break;
                case LFCMXML_ARGTYPE_XMLSTRING:
                    lfcHashmap_put(args, (intptr_t *)LFCMXML_ARGTYPE_XMLSTRING, (char *) va_arg(ap, const char *));
                    break;

                default:
                    lfcLogger_ERR_g("[lfcMXML] unknown arg type: %d", currentArgType);
                    goto err;
            }
            currentArgType = va_arg(ap, lfcMXML_argType_e);
        }

        va_end(ap);
    }

    // checks
    {
        if (!lfcIIterable_exists(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_CALLBACK)) {
            lfcLogger_ERR_g("arg LFCMXML_ARGTYPE_MAINNODE_CALLBACK not found");
            goto err;
        }
    }

    // parse arguments und exec!
    {
        logger = asInstanceOf(lfcLogger(), lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_LOGGER));
        logger = (logger) ? logger : lfcLogger_ctor_wGlobalLogHandler(lfcMXML_LOGGER_PREFIX);

        result = lfcList_ctor();
        if (!result) {
            lfcLogger_ERR(logger, "could not create result list");
            goto err;
        }

        if (lfcIIterable_exists(args, (intptr_t *)LFCMXML_ARGTYPE_XMLSTRING)) {
            lfcList_mergeList(
                result,
                private_lfcMXMLParser_parse_xmlString(
                    logger,
                    lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_XMLSTRING),
                    lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_NAME),
                    lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_CALLBACK)
                )
            );
        }
        if (lfcIIterable_exists(args, (intptr_t *)LFCMXML_ARGTYPE_XMLFILE)) {
            lfcList_mergeList(
                result,
                private_lfcMXMLParser_parse_xmlFile(
                    logger,
                    lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_XMLFILE),
                    lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_NAME),
                    lfcHashmap_get(args, (intptr_t *)LFCMXML_ARGTYPE_MAINNODE_CALLBACK)
                )
            );
        }

        // wenn wir den logger angelegt haben, wieder löschen
        const char *loggerPrefix = lfcLogger_getPrefix(logger);
        if (strlen(loggerPrefix) == strlen(lfcMXML_LOGGER_PREFIX) && 0 == strcmp(loggerPrefix, lfcMXML_LOGGER_PREFIX)) {
            delete(logger);
        }
    }

    delete(args);

    return result;

err:
    if (args) { delete(args); }
    if (result) { delete(result); }
    if (logger) {
        const char *loggerPrefix = lfcLogger_getPrefix(logger);
        if (strlen(loggerPrefix) == strlen(lfcMXML_LOGGER_PREFIX) && 0 == strcmp(loggerPrefix, lfcMXML_LOGGER_PREFIX)) {
            delete(logger);
        }
    }

    return NULL;
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

