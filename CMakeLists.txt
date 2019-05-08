cmake_minimum_required(VERSION 3.8)
project(libForC)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -fno-pie -no-pie -g -ggdb -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -fno-pie -no-pie -g -ggdb -Wall -Werror -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast")
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(SOURCE_FILES
        src/main/clib/lfcTimerfd.h
        src/main/clib/lfcTimerfd.c
        src/main/collection/lfcBitset.h
        src/main/collection/lfcBitset.c
        src/main/collection/lfcByteBuffer.h
        src/main/collection/lfcByteBuffer.c
        src/main/collection/lfcHashmap.h
        src/main/collection/lfcHashmap.c
        src/main/collection/lfcIIterable.h
        src/main/collection/lfcIIterable.c
        src/main/collection/lfcList.h
        src/main/collection/lfcList.c
        src/main/collection/lfcQueue.h
        src/main/collection/lfcQueue.c
        src/main/collection/lfcTuple.h
        src/main/collection/lfcTuple.c
        src/main/core/lfcClosure.h
        src/main/core/lfcCore.h
        src/main/core/lfcIface.h
        src/main/core/lfcObject.h
        src/main/core/lfcObject.c
        src/main/data/lfcStringOps.c
        src/main/data/lfcStringOps.h
        src/main/io/lfcIOOps.c
        src/main/io/lfcIOOps.h
        src/main/ipc/lfcMessage.c
        src/main/ipc/lfcMessage.h
        src/main/ipc/lfcMQueueReceiver.c
        src/main/ipc/lfcMQueueReceiver.h
        src/main/ipc/lfcMQueueSender.c
        src/main/ipc/lfcMQueueSender.h
        src/main/mem/lfcMemPool.h
        src/main/mem/lfcMemPool.c
        src/main/net/lfcSocket.c
        src/main/net/lfcSocket.h
        src/main/net/lfcSocketHandler.c
        src/main/net/lfcSocketHandler.h
        src/main/net/lfcSocketJob.c
        src/main/net/lfcSocketJob.h
        src/main/net/lfcSocketJobAcceptConn.c
        src/main/net/lfcSocketJobAcceptConn.h
        src/main/net/lfcSocketJobReader.c
        src/main/net/lfcSocketJobReader.h
        src/main/net/lfcSocketJobReaderStream.c
        src/main/net/lfcSocketJobReaderStream.h
        src/main/net/lfcSocketJobWriter.c
        src/main/net/lfcSocketJobWriter.h
        src/main/testing/lfcCriterionHelper.h
        src/main/testing/lfcCriterionHelper.c
        src/main/testing/lfcCriterionObject.h
        src/main/threading/lfcActor.c
        src/main/threading/lfcActor.h
        src/main/threading/lfcActorRef.c
        src/main/threading/lfcActorRef.h
        src/main/threading/lfcActorMessage.c
        src/main/threading/lfcActorMessage.h
        src/main/threading/lfcActorSystem.c
        src/main/threading/lfcActorSystem.h
        src/main/threading/lfcThreadMisc.h
        src/main/threading/lfcThreadMisc.c
        src/main/threading/lfcThreadPool.c
        src/main/threading/lfcThreadPool.h
        src/main/tools/lfcLogger.c
        src/main/tools/lfcLogger.h
        src/main/tools/lfcLogInstance.c
        src/main/tools/lfcLogInstance.h
        src/main/tools/lfcOptParser.c
        src/main/tools/lfcOptParser.h
        src/main/tools/lfcOptParserCmd.c
        src/main/tools/lfcOptParserCmd.h
        src/main/tools/lfcOptParserParam.c
        src/main/tools/lfcOptParserParam.h
        )

set(SOURCE_FILES_unitTests
        src/test/collection/lfcBitset/spec_lfcBitset_clear.c
        src/test/collection/lfcBitset/spec_lfcBitset_count.c
        src/test/collection/lfcBitset/spec_lfcBitset_ctor.c
        src/test/collection/lfcBitset/spec_lfcBitset_ctor_withBits.c
        src/test/collection/lfcBitset/spec_lfcBitset_get.c
        src/test/collection/lfcBitset/spec_lfcBitset_set.c
        src/test/collection/lfcBitset/spec_lfcBitset_toggle.c

        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_ctor.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_capacity.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_isEmpty.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_nonEmpty.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_int8.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_int16.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_int32.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_int64.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_uint8.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_uint16.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_uint32.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_uint64.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_read_int8ptr.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_int8.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_int16.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_int32.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_int64.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_uint8.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_uint16.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_uint32.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_uint64.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_write_int8ptr.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_init_fromArray.c
        src/test/collection/lfcByteBuffer/spec_lfcByteBuffer_toArray.c

        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_count.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_exists.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_filter.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_filterNot.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_find.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_foreach.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_head.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_isEmpty.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_last.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_map.c
        src/test/collection/lfcHashmap/spec_lfcHashmap__lfcIIterable_nonEmpty.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_clear.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_ctor.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_ctorWithSize.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_dtor.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_get.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_methods.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_put.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_remove.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_wIntKeys.c
        src/test/collection/lfcHashmap/spec_lfcHashmap_wStringKeys.c

        src/test/collection/lfcList/spec_lfcList__lfcIIterable_count.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_exists.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_filter.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_filterNot.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_find.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_foreach.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_isEmpty.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_head.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_last.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_map.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_next.c
        src/test/collection/lfcList/spec_lfcList__lfcIIterable_nonEmpty.c
        src/test/collection/lfcList/spec_lfcList_add.c
        src/test/collection/lfcList/spec_lfcList_clear.c
        src/test/collection/lfcList/spec_lfcList_ctor.c
        src/test/collection/lfcList/spec_lfcList_ctorWithSize.c
        src/test/collection/lfcList/spec_lfcList_dtor.c
        src/test/collection/lfcList/spec_lfcList_getAt.c
        src/test/collection/lfcList/spec_lfcList_indexOf.c
        src/test/collection/lfcList/spec_lfcList_insertAt.c
        src/test/collection/lfcList/spec_lfcList_methods.c
        src/test/collection/lfcList/spec_lfcList_remove.c
        src/test/collection/lfcList/spec_lfcList_removeAt.c

        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_count.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_exists.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_filter.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_filterNot.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_find.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_foreach.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_isEmpty.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_head.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_last.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_map.c
        src/test/collection/lfcQueue/spec_lfcQueue_lfcIIterable_nonEmpty.c
        src/test/collection/lfcQueue/spec_lfcQueue_clear.c
        src/test/collection/lfcQueue/spec_lfcQueue_ctor.c
        src/test/collection/lfcQueue/spec_lfcQueue_ctorWithSize.c
        src/test/collection/lfcQueue/spec_lfcQueue_dtor.c
        src/test/collection/lfcQueue/spec_lfcQueue_enqueue.c
        src/test/collection/lfcQueue/spec_lfcQueue_dequeue.c
        src/test/collection/lfcQueue/spec_lfcQueue_methods.c

        src/test/core/spec_lfcObject_asInstanceOf.c
        src/test/core/spec_lfcObject_isInstanceOf.c
        src/test/core/spec_lfcObject_new.c
        src/test/core/mocks/ClassA.h
        src/test/core/mocks/ClassA.c

        src/test/data/lfcStringOps/spec_lfcStringOps_replace.c

        src/test/ipc/lfcMQueueReceiver/spec_lfcMQueueReceiver_ctorSimple.c
        src/test/ipc/lfcMQueueReceiver/spec_lfcMQueueReceiver_receive.c

        src/test/mem/lfcMemPool/spec_lfcMemPool_alloc_oneBlock.c
        src/test/mem/lfcMemPool/spec_lfcMemPool_alloc_moreBlocks.c
        src/test/mem/lfcMemPool/spec_lfcMemPool_calloc.c
        src/test/mem/lfcMemPool/spec_lfcMemPool_ctor.c
        src/test/mem/lfcMemPool/spec_lfcMemPool_dtor.c
        src/test/mem/lfcMemPool/spec_lfcMemPool_free_moreBlocks.c
        src/test/mem/lfcMemPool/spec_lfcMemPool_next.c

        src/test/net/lfcSocket/spec_lfcSocket_dtor.c
        src/test/net/lfcSocket/spec_lfcSocket_read.c
        src/test/net/lfcSocket/spec_lfcSocket_read_async.c
        src/test/net/lfcSocket/spec_lfcSocket_write.c
        src/test/net/lfcSocket/spec_lfcSocket_write_async.c
        src/test/net/lfcSocket/spec_lfcSocket_write_realWorld_01.c
        src/test/net/lfcSocket/spec_lfcSocket_write_realWorld_02.c
        src/test/net/lfcSocketHandler/spec_lfcSocketHandler_crashDetection.c
        src/test/net/lfcSocketHandler/spec_lfcSocketHandler_singleton.c
        src/test/net/lfcSocketHandler/spec_lfcSocketHandler_read.c
        src/test/net/lfcSocketHandler/spec_lfcSocketHandler_write.c
        src/test/net/lfcSocketJob/spec_lfcSocketJob_ctor.c
        src/test/net/lfcSocketJob/spec_lfcSocketJob_disable.c
        src/test/net/lfcSocketJob/spec_lfcSocketJob_enable.c
        src/test/net/lfcSocketJob/spec_lfcSocketJob_isEnabled.c
        src/test/net/lfcSocketJobReader/spec_lfcSocketJobReader_ctor.c
        src/test/net/lfcSocketJobReader/spec_lfcSocketJobReader_ctor_wBuffer.c
        src/test/net/lfcSocketJobReader/spec_lfcSocketJobReader_ctor_wRepeat.c
        src/test/net/lfcSocketJobReader/spec_lfcSocketJobReader_doWork.c
        src/test/net/lfcSocketJobReaderStream/spec_lfcSocketJobReaderStream_ctor.c
        src/test/net/lfcSocketJobReaderStream/spec_lfcSocketJobReaderStream_ctor_wBuffer.c
        src/test/net/lfcSocketJobReaderStream/spec_lfcSocketJobReaderStream_doWork.c
        src/test/net/lfcSocketJobWriter/spec_lfcSocketJobWriter_ctor.c
        src/test/net/lfcSocketJobWriter/spec_lfcSocketJobWriter_doWork.c

        src/test/threading/lfcActor/spec_lfcActor_ctor.c
        src/test/threading/lfcActor/spec_lfcActor_getActorSystem.c
        src/test/threading/lfcActor/spec_lfcActor_getName.c
        src/test/threading/lfcActor/spec_lfcActor_getReceiveFn.c
        src/test/threading/lfcActor/spec_lfcActor_tell.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_ctor.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_ctor_noSender.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_getMsg.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_getMsgLen.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_getRecipient.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_getSender.c
        src/test/threading/lfcActorMessage/spec_lfcActorMessage_hasSender.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_ctor.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_create.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_getName.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_equals.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_equals_byActor.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_equals_byActorRef.c
        src/test/threading/lfcActorSystem/spec_lfcActorSystem_tell.c
        )

set(SOURCE_FILES_runner
        src/test/runner.c)

link_directories(
        src/test/__needed_libs__/32bit
        src/test/__needed_libs__/64bit
)

add_executable(libForC_ut ${SOURCE_FILES} ${SOURCE_FILES_unitTests})
target_link_libraries(libForC_ut criterion rt pthread)

add_executable(libForC_runner ${SOURCE_FILES} ${SOURCE_FILES_runner})
target_link_libraries(libForC_runner rt pthread)

include_directories(src/main)
