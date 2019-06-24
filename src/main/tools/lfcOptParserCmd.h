#ifndef LIBFORC_TOOLS_OPTPARSER_CMD_H
#define LIBFORC_TOOLS_OPTPARSER_CMD_H

#include <stdint.h>
#include <pthread.h>
#include "../collection/lfcList.h"
#include "../collection/lfcIIterable.h"
#include "../core/lfcObject.h"
#include "lfcOptParserParam.h"

DEFINE_CLASS(lfcOptParserCmd)

struct lfcOptParserCmd { const struct lfcObject _;
    char *name;
    char *help;

    lfcList_t *cmdList;
    lfcList_t *paramList;
};

struct lfcOptParserCmd_class { const struct lfcObject_class _;
    method_t addCmd;
    method_t addParam;
    method_t parse;
};

struct lfcOptParserCmd_methods {
    int (*addCmd  )(lfcOptParserCmd_t *self, lfcOptParserCmd_t *toAdd);
    int (*addParam)(lfcOptParserCmd_t *self, lfcOptParserParam_t *toAdd);
    int (*parse   )(lfcOptParserCmd_t *self, const char *in);

    // super
    const lfcObject_methods_t *base;

    // ifaces
};

lfcOptParserCmd_t *lfcOptParserCmd_ctor (
    const char *name,
    const char *help
);

int lfcOptParserCmd_addCmd(lfcOptParserCmd_t *self, lfcOptParserCmd_t *toAdd);
int lfcOptParserCmd_addParam(lfcOptParserCmd_t *self, lfcOptParserParam_t *toAdd);
int lfcOptParserCmd_parse(lfcOptParserCmd_t *self, const char *in);

lfcOptParserCmd_t *lfcOptParserCmd_hlpr_addCmd (
    lfcOptParserCmd_t *self,
    const char *name,
    const char *help
);

lfcOptParserParam_t *lfcOptParserCmd_hlpr_addParam (
    lfcOptParserCmd_t *self,
    const char *name,
    const char *help
);


#endif //LIBFORC_TOOLS_OPTPARSER_CMD_H
