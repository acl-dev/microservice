cmake_minimum_required(VERSION 2.8)

project(http_rpc)

IF(NOT ACL_ROOT)
	MESSAGE(FATAL_ERROR "need define ACL_ROOT")
ENDIF()

INCLUDE_DIRECTORIES(${ACL_ROOT}/lib_acl/include)

INCLUDE_DIRECTORIES(${ACL_ROOT}/lib_acl_cpp/include)

INCLUDE_DIRECTORIES(${http_rpc_SOURCE_DIR}/include)

aux_source_directory(${http_rpc_SOURCE_DIR}/src SRC_LIST)

ADD_LIBRARY(http_rpc STATIC ${SRC_LIST})

