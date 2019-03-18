#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"
#include "testing/lfcCriterionObject.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_methods

TEST_CLASS_METHODS(lfcQueue, clear)
TEST_CLASS_METHODS(lfcQueue, enqueue)
TEST_CLASS_METHODS(lfcQueue, dequeue)

TEST_CLASS_SUPER_METHODS(lfcQueue, lfcObject)
TEST_CLASS_IFACE_METHODS(lfcQueue, lfcIIterable)
