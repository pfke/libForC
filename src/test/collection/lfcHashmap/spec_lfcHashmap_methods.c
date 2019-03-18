#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"
#include "testing/lfcCriterionObject.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_methods

TEST_CLASS_METHODS(lfcHashmap, clear)
TEST_CLASS_METHODS(lfcHashmap, get)
TEST_CLASS_METHODS(lfcHashmap, put)
TEST_CLASS_METHODS(lfcHashmap, remove)

TEST_CLASS_SUPER_METHODS(lfcHashmap, lfcObject)
TEST_CLASS_IFACE_METHODS(lfcHashmap, lfcIIterable)
