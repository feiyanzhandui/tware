add_executable (webjob webjob.cpp)
target_link_libraries (webjob
    ${PROTOBUF_LIB}
    ${ZMQ_LIB}
    ${LLVM_LIB}
    proto
)