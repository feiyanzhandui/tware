add_executable (frontend_test frontend_test.cpp)
target_link_libraries (frontend_test
    ${ZMQ_LIB}
    ${LLVM_LIB}
)

#add_executable (kmeans_test kmeans_test.cpp)
#target_link_libraries (kmeans_test
#    ${ZMQ_LIB}
#    ${LLVM_LIB}
#)
