add_executable (load_test load.cpp)
target_link_libraries (load_test
    ${PROTOBUF_LIB}
    ${ZMQ_LIB}
    proto
)

add_executable (logreg_test logreg.cpp)
target_link_libraries (logreg_test
    ${PROTOBUF_LIB}
    ${ZMQ_LIB}
    proto
)

add_executable (pagerank_test pagerank.cpp)
target_link_libraries (pagerank_test
    ${PROTOBUF_LIB}
    ${ZMQ_LIB}
    proto
)

add_executable (sort_test sort.cpp)
target_link_libraries (sort_test
    ${PROTOBUF_LIB}
    ${ZMQ_LIB}
    proto
)
