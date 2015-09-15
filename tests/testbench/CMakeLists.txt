add_executable (testbench testbench.cpp)
target_link_libraries(testbench 
    ${GTEST_LIBRARIES}
    ${ZMQ_LIB}
    pthread dl m
    proto
)
