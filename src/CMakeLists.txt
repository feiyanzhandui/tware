add_executable (tw-master Master.cpp)
target_link_libraries (tw-master
    ${CLANG_LIB}
#    ${HDFS_LIB}
    ${LLVM_LIB}
    ${ZMQ_LIB}
    pthread dl m z
)

add_executable (tw-worker Worker.cpp)
target_link_libraries (tw-worker
    ${CLANG_LIB}
#    ${HDFS_LIB}
    ${LLVM_LIB}
    ${ZMQ_LIB}
    pthread dl m z
)
