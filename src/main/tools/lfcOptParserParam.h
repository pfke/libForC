#ifndef LIBFORC_TOOLS_OPTPARSER_PARAM_H
#define LIBFORC_TOOLS_OPTPARSER_PARAM_H

#include <stdint.h>
#include <pthread.h>
#include <collection/lfcList.h>
#include "../collection/lfcIIterable.h"
#include "../core/lfcObject.h"

DEFINE_CLASS(lfcOptParserParam)

struct lfcOptParserParam { const struct lfcObject _;
    char *name;
    char *help;
};

struct lfcOptParserParam_class { const struct lfcObject_class _;
    method_t parse;
};

struct lfcOptParserParam_methods {
    int (*parse   )(lfcOptParserParam_t *self, const char *in);

    // super
    const lfcObject_methods_t *base;

    // ifaces
};

lfcOptParserParam_t *lfcOptParserParam_ctor (
    const char *name,
    const char *help
);

int lfcOptParserParam_parse(lfcOptParserParam_t *self, const char *in);


#endif //LIBFORC_TOOLS_OPTPARSER_PARAM_H
