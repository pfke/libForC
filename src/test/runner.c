#include <stdio.h>
#include <tools/lfcOptParser.h>
#include <data/lfcStringOps.h>
#include <malloc.h>
#include <mem/lfcMemPool.h>
#include <net/lfcSocket.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocketJobReaderStream.h>
#include <threading/lfcThreadPool.h>
#include <threading/actor/lfcActorSystem.h>
#include <net/lfcSocketJobAcceptConn.h>

#include "tools/logging/lfcLogConsoleAppender.h"
#include "tools/logging/lfcLogger.h"
#include "tools/logging/lfcLogHandler.h"

#include "io/lfcMXMLParser.h"

#include <mxml.h>

#define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))
#define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

typedef struct test_wlan_finalize_forIntel_caseClass_forBlob {
    char *descr;

    int deviceCalVer;
    char *deviceCalFile;

    int designCalVer;
    char *designCalFile;

    char *setconfigBand;

    /**
     * Alle in demstruct angegebenen const char * verweisen auf diesen Daten-Bereich.
     */
    char data[0];
} test_wlan_finalize_forIntel_caseClass_forBlob_t;

typedef struct test_wlan_finalize_forIntel_caseClass_forXmlData {
    int productCalVer;
    char *urladerMTD;

    lfcList_t *blobs;

    /**
     * Alle in demstruct angegebenen const char * verweisen auf diesen Daten-Bereich.
     */
    char data[0];
} test_wlan_finalize_forIntel_caseClass_forXmlData_t;

test_wlan_finalize_forIntel_caseClass_forBlob_t *test_wlan_finaliize_forIntel_parseXmlNode_blob(
    lfcLogger_t *logger,
    mxml_node_t *node
) {
#define TWFFI_BLOB_NODENAME                     "blob"
#define TWFFI_BLOB_NODENAME_ATTR_DESCR          "descr"
#define TWFFI_GETCAL_NODENAME                   "getcal"
#define TWFFI_DEVICECAL_NODENAME                "deviceCal"
#define TWFFI_DEVICECAL_NODENAME_ATTR_VER       "ver"
#define TWFFI_DEVICECAL_NODENAME_ATTR_FILE      "file"
#define TWFFI_DESIGNCAL_NODENAME                "designCal"
#define TWFFI_DESIGNCAL_NODENAME_ATTR_VER       "ver"
#define TWFFI_DESIGNCAL_NODENAME_ATTR_FILE      "file"
#define TWFFI_SETCONFIG_NODENAME                "setconfig"
#define TWFFI_SETCONFIG_NODENAME_ATTR_BAND      "band"

    if (!logger) { return NULL; }
    if (!node) {
        lfcLogger_ERR(logger, "argument node is NULL");
        goto err;
    }
    if (strlen(TWFFI_BLOB_NODENAME) != strlen(node->value.element.name) || 0 != strncmp(node->value.element.name, TWFFI_BLOB_NODENAME, strlen(TWFFI_BLOB_NODENAME))) {
        lfcLogger_ERR(logger, "wrong node. Node w/ name '%s' expected, but got '%s'", TWFFI_BLOB_NODENAME, node->value.element.name);
        goto err;
    }

    // read descr
    const char *descr = mxmlElementGetAttr(node, TWFFI_BLOB_NODENAME_ATTR_DESCR);

    // read getcal
    mxml_node_t *getCalNode = mxmlFindElement(node, node, TWFFI_GETCAL_NODENAME, NULL, NULL, MXML_DESCEND_FIRST);
    if (!getCalNode) {
        lfcLogger_ERR(logger, "'%s' not found", TWFFI_GETCAL_NODENAME);
        goto err;
    }

    // read deviceCal
    const char *deviceCalVer;
    const char *deviceCalFile;
    {
        mxml_node_t *deviceCalNode = mxmlFindElement(getCalNode, getCalNode, TWFFI_DEVICECAL_NODENAME, NULL, NULL, MXML_DESCEND_FIRST);
        if (!deviceCalNode) {
            lfcLogger_ERR(logger, "'%s' not found", TWFFI_DEVICECAL_NODENAME);
            goto err;
        }

        // read deviceCalVer
        deviceCalVer = mxmlElementGetAttr(deviceCalNode, TWFFI_DEVICECAL_NODENAME_ATTR_VER);
        deviceCalFile = mxmlElementGetAttr(deviceCalNode, TWFFI_DEVICECAL_NODENAME_ATTR_FILE);
    }

    // read designCal
    const char *designCalVer;
    const char *designCalFile;
    {
        mxml_node_t *designCalNode = mxmlFindElement(getCalNode, getCalNode, TWFFI_DESIGNCAL_NODENAME, NULL, NULL, MXML_DESCEND_FIRST);
        if (!designCalNode) {
            lfcLogger_ERR(logger, "'%s' not found", TWFFI_DESIGNCAL_NODENAME);
            goto err;
        }

        // read designCalVer
        designCalVer = mxmlElementGetAttr(designCalNode, TWFFI_DESIGNCAL_NODENAME_ATTR_VER);
        designCalFile = mxmlElementGetAttr(designCalNode, TWFFI_DESIGNCAL_NODENAME_ATTR_FILE);
    }

    // read setconfig
    const char *setconfigBand;
    {
        mxml_node_t *setconfigNode = mxmlFindElement(node, node, TWFFI_SETCONFIG_NODENAME, NULL, NULL, MXML_DESCEND_FIRST);
        if (!setconfigNode) {
            lfcLogger_ERR(logger, "'%s' not found", TWFFI_SETCONFIG_NODENAME);
            goto err;
        }

        // read setconfigBand
        setconfigBand = mxmlElementGetAttr(setconfigNode, TWFFI_SETCONFIG_NODENAME_ATTR_BAND);
    }

    // create case class
    size_t needed_size = 0 +
        sizeof(test_wlan_finalize_forIntel_caseClass_forBlob_t) +
        strlen(descr) + 1 +
        strlen(designCalFile) + 1 +
        strlen(deviceCalFile) + 1 +
        strlen(setconfigBand) + 1 +
        0;
    test_wlan_finalize_forIntel_caseClass_forBlob_t *caseClass = calloc(1, needed_size);
    if (!caseClass) {
        lfcLogger_ERR(logger, "not enough mem to alloc case class. Needed %zu byte", needed_size);
        goto err;
    }

    caseClass->descr = caseClass->data;
    strcat(caseClass->descr, descr);

    caseClass->designCalVer = strtoul(designCalVer, NULL, 10);
    caseClass->designCalFile = caseClass->data + strlen(caseClass->descr) + 1;
    strcat(caseClass->designCalFile, designCalFile);

    caseClass->deviceCalVer = strtoul(deviceCalVer, NULL, 10);
    caseClass->deviceCalFile = caseClass->designCalFile + strlen(caseClass->designCalFile) + 1;
    strcat(caseClass->designCalFile, designCalFile);

    caseClass->setconfigBand = caseClass->deviceCalFile + strlen(caseClass->deviceCalFile) + 1;
    strcat(caseClass->setconfigBand, setconfigBand);

    lfcLogger_DEBUG(
        logger,
        "read blob w/ following data:          \n"
        "  blob                                \n"
        "  ----                                \n"
        "    descr: '%s'                       \n"
        "    getcal                            \n"
        "      deviceCal: version=%d, file='%s'\n"
        "      designCal: version=%d, file='%s'\n"
        "      setconfigBand: '%s'               ",
        caseClass->descr,
        caseClass->deviceCalVer, caseClass->deviceCalFile,
        caseClass->designCalVer, caseClass->designCalFile,
        caseClass->setconfigBand
    );

    return caseClass;

err:
    return NULL;
}

void *test_wlan_finaliize_forIntel_parseXml(
    lfcLogger_t *logger,
    mxml_node_t *node
) {
#define TWFFI_MAIN_NODENAME                     "wlanFinalize"
#define TWFFI_MAIN_NODENAME_ATTR_PRODCALVER     "productionCalVer"
#define TWFFI_MAIN_NODENAME_ATTR_URLADERMTD     "urladerMTD"
#define TWFFI_BLOB_NODENAME     "blob"

    if (!logger) { return NULL; }
    if (!node) {
        lfcLogger_ERR(logger, "argument node is NULL");
        return NULL;
    }
    if (strlen(TWFFI_MAIN_NODENAME) != strlen(node->value.element.name) || 0 != strncmp(node->value.element.name, TWFFI_MAIN_NODENAME, strlen(TWFFI_MAIN_NODENAME))) {
        lfcLogger_ERR(logger, "wrong node. Node w/ name '%s' expected, but got '%s'", TWFFI_MAIN_NODENAME, node->value.element.name);
        return NULL;
    }

    // read productionCalVer
    const char *productionCalVer = mxmlElementGetAttr(node, TWFFI_MAIN_NODENAME_ATTR_PRODCALVER);
    // read productionCalVer
    const char *urladerMTD = mxmlElementGetAttr(node, TWFFI_MAIN_NODENAME_ATTR_URLADERMTD);

    size_t needed_size = 0 +
        sizeof(test_wlan_finalize_forIntel_caseClass_forXmlData_t) +
        strlen(urladerMTD) + 1 +
        0;
    test_wlan_finalize_forIntel_caseClass_forXmlData_t *result = calloc(1, needed_size);
    if (!result) {
        lfcLogger_ERR(logger, "could not alloc mem for result. Needed %zu byte", needed_size);
        goto err;
    }
    result->blobs = lfcList_ctor();
    if (!result->blobs) {
        lfcLogger_ERR(logger, "could not create result blob list");
        goto err;
    }

    result->productCalVer = strtoul(productionCalVer, NULL, 10);
    result->urladerMTD = result->data;
    strcat(result->urladerMTD, urladerMTD);

    // process all blobs
    mxml_node_t *child;
    for (
        child = mxmlWalkNext(node, node, MXML_DESCEND_FIRST);
        NULL != child;
        child = mxmlWalkNext(child, node, MXML_NO_DESCEND)
        ) {
        if (child->type != MXML_ELEMENT) { continue; }

        const char *nodeName = child->value.element.name;
        if (strlen(nodeName) != strlen(TWFFI_BLOB_NODENAME) && 0 != strncmp(nodeName, TWFFI_BLOB_NODENAME, strlen(TWFFI_BLOB_NODENAME))) {
            lfcLogger_DEBUG(logger, "ignore node: '%s'", child->value.element.name);
            continue;
        }

        lfcLogger_DEBUG(logger, "found node: '%s'", child->value.element.name);
        test_wlan_finalize_forIntel_caseClass_forBlob_t *blob = test_wlan_finaliize_forIntel_parseXmlNode_blob(logger, child);
        if (!blob) {
            lfcLogger_ERR(logger, "error parsing blob");
            goto err;
        }

        lfcList_add(result->blobs, blob);
    }

    lfcLogger_DEBUG(
        logger,
        "read complete wlanFinalizeData w/ following data:\n"
        "  data                                           \n"
        "  ----                                           \n"
        "    productCal: version=%d                       \n"
        "    urladerMTD: '%s'                             \n"
        "    %d blob(s) in total                            ",
        result->productCalVer,
        result->urladerMTD,
        lfcIIterable_count(result->blobs)
    );

    return result;

err:
    if (result) {
        if (result->blobs) { delete(result->blobs); }
        free(result);
    }

    return NULL;
}

void runner_fn () {
    lfcLogHandler_t *logHandler = lfcLogHandler_singleton();
    lfcLogger_t *logger = lfcLogger_ctor(logHandler, "mxmlParserTest");

    lfcLogHandler_addAppender(logHandler, asInstanceOf(lfcLogAppender(), lfcLogConsoleAppender_ctor("%c%D{%F %T} [%l] %p --- [%t->%T] --- %F:%f : %m%C")));
    lfcLogHandler_setLogLevel(logHandler, LOGLEVEL_DEBUG);

    const char *xmlString =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<wlanFinalize productionCalVer=\"7\" urladerMTD=\"/dev/mtd3\">"
        "    <blob descr=\"2.4GHz cascade - dies ist nur ein Beschreibungstext\">"
        "        <getcal>"
        "            <deviceCal ver=\"1\" file=\"/var/tmp/wlancal/cal_wlan0.data\" />"
        "            <designCal ver=\"2\" file=\"/var/tmp/wlancal/psd.in\" />"
        "        </getcal>"
        "        <setconfig band=\"w2\" />"
        "    </blob>"
        "    <blob descr=\"5GHz cascade - dies ist nur ein Beschreibungstext\">"
        "        <getcal>"
        "            <deviceCal ver=\"3\" file=\"/var/tmp/wlancal/cal_wlan1.data\" />"
        "            <designCal ver=\"4\" file=\"/var/tmp/wlancal/psd.in\" />"
        "        </getcal>"
        "        <setconfig band=\"w3\" />"
        "    </blob>"
        "    <blob descr=\"5GHz chocolate - dies ist nur ein Beschreibungstext\">"
        "        <getcal>"
        "            <deviceCal ver=\"5\" file=\"/var/tmp/wlancal/cal_wlan2.data\" />"
        "            <designCal ver=\"6\" file=\"/var/tmp/wlancal/psd.in\" />"
        "        </getcal>"
        "        <setconfig band=\"w\" />"
        "    </blob>"
        "</wlanFinalize>"
        "";

    lfcLogger_INFO(logger, "callback=%p", test_wlan_finaliize_forIntel_parseXml);
    lfcList_t *result = lfcMXMLParser_parse(
        LFCMXML_ARGTYPE_XMLSTRING, xmlString,
        LFCMXML_ARGTYPE_XMLFILE, "/home/hblobner/test.xml",
        LFCMXML_ARGTYPE_LOGGER, logger,
        LFCMXML_ARGTYPE_MAINNODE_NAME, "wlanFinalize",
        LFCMXML_ARGTYPE_MAINNODE_CALLBACK, test_wlan_finaliize_forIntel_parseXml,
        NULL
    );

    test_wlan_finalize_forIntel_caseClass_forXmlData_t *data0 = lfcList_getAt(result, 0);
    test_wlan_finalize_forIntel_caseClass_forXmlData_t *data1 = lfcList_getAt(result, 1);

    fprintf(stderr, "%s@%d\n", __func__,  __LINE__);
}

int main (
    int argc __attribute__((unused)),
    char *argv[] __attribute__((unused))
) {
    runner_fn();

    return 0;
}
