# Tupleware. Copyright (C) 2013-2015. <License>.

#
# Project info
#
cmake_minimum_required (VERSION 2.8.4)
project (TUPLEWARE)
set (TUPLEWARE_VERSION_MAJOR 0)
set (TUPLEWARE_VERSION_MINOR 1)

#
# Debug
#
set (TUPLEWARE_VERBOSE_MAKEFILE 0)

#
# Build
#
set (LIBRARY_OUTPUT_PATH ${TUPLEWARE_BINARY_DIR}/bin)
set (EXECUTABLE_OUTPUT_PATH ${TUPLEWARE_BINARY_DIR}/bin)
set (CMAKE_CXX_FLAGS "-std=c++1y -O3")

#
# CLANG
#
#LIST (APPEND CLANG_LIB "clangFrontendTool" "clangFrontend" "clangDriver" "clangSerialization" "clangCodeGen" "clangParse" "clangSema" "clangStaticAnalyzerFrontend" "clangStaticAnalyzerCheckers" "clangStaticAnalyzerCore" "clangAnalysis" "clangARCMigrate" "clangRewriteFrontend" "clangEdit" "clangAST" "clangLex" "clangBasic")

#
# HDFS
#
find_path (HDFS_INCLUDE NAMES hdfs.h PATHS /usr/local/hadoop/include)
find_library (HDFS_LIB NAMES hdfs PATHS /usr/local/hadoop/lib/native)
include_directories (${HDFS_INCLUDE})
link_directories (${HDFS_LIB})

#
# LLVM
#
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIB all)

#
# ZMQ
#
find_path (ZMQ_INCLUDE NAMES zmq.h PATHS /usr/include/ /usr/local/include/)
find_library (ZMQ_LIB NAMES zmq PATHS /usr/lib /usr/local/lib)
include_directories (${ZMQ_INCLUDE})
link_directories (${ZMQ_LIB})

#
# GTest
#
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories (
    ${TUPLEWARE_SOURCE_DIR}
    ${TUPLEWARE_SOURCE_DIR}/src
    ${TUPLEWARE_SOURCE_DIR}/src/message
)

add_subdirectory (src)
#add_subdirectory (tests/_stored)
add_subdirectory (tests/frontend)
#add_subdirectory (tests/_webjob)
