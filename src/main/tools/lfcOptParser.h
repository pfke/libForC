#ifndef LIBFORC_TOOLS_OPTPARSER_H
#define LIBFORC_TOOLS_OPTPARSER_H

#include <stdint.h>
#include <pthread.h>
#include "../collection/lfcList.h"
#include "../collection/lfcIIterable.h"
#include "../core/lfcObject.h"
#include "lfcOptParserCmd.h"

DEFINE_CLASS(lfcOptParser)

/**
 * Dieser Callback wird aufgerufen, wenn das Kommando geparst wurde
 */
typedef int (*lfcOptParser_cmdParsed_cb)(lfcOptParserCmd_t *cmd, lfcList_t *params);

struct lfcOptParser { const struct lfcObject _;
    lfcList_t *cmdList;
    lfcList_t *paramList;
};

struct lfcOptParser_class { const struct lfcObject_class _;
    method_t addCmd;
    method_t addParam;
    method_t dump;
    method_t parse;
};

struct lfcOptParser_methods {
    int (*addCmd  )(lfcOptParser_t *self, lfcOptParserCmd_t *toAdd);
    int (*addParam)(lfcOptParser_t *self, lfcOptParserParam_t *toAdd);
    int (*dump )(lfcOptParser_t *self, FILE *fp);
    int (*parse)(lfcOptParser_t *self, const char *in);

    // super
    const lfcObject_methods_t *base;

    // ifaces
};

lfcOptParser_t *lfcOptParser_ctor();

int lfcOptParser_addCmd(lfcOptParser_t *self, lfcOptParserCmd_t *toAdd);
int lfcOptParser_addParam(lfcOptParser_t *self, lfcOptParserParam_t *toAdd);
int lfcOptParser_dump(lfcOptParser_t *self, FILE *fp);
int lfcOptParser_parse(lfcOptParser_t *self, const char *in);

lfcOptParserCmd_t *lfcOptParser_hlpr_addCmd (
    lfcOptParser_t *self,
    const char *name,
    const char *help
);

lfcOptParserParam_t *lfcOptParser_hlpr_addParam (
    lfcOptParser_t *self,
    const char *name,
    const char *help
);


#endif //LIBFORC_TOOLS_OPTPARSER_H
