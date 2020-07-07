#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"
#include "testing/lfcCriterionObject.h"

#define TEST_SUITE_NAME               lfcList__lfcList_methods

TEST_CLASS_METHODS(lfcList, add)
TEST_CLASS_METHODS(lfcList, clear)
TEST_CLASS_METHODS(lfcList, getAt)
TEST_CLASS_METHODS(lfcList, indexOf)
TEST_CLASS_METHODS(lfcList, remove)
TEST_CLASS_METHODS(lfcList, removeAt)

TEST_CLASS_SUPER_METHODS(lfcList, lfcObject)
TEST_CLASS_IFACE_METHODS(lfcList, lfcIIterable)
