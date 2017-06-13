cmake_minimum_required(VERSION 2.8)

project(http_rpc)

INCLUDE_DIRECTORIES(${http_rpc_SOURCE_DIR}/include)

IF(NOT ACL_ROOT)
	MESSAGE(FATAL_ERROR "need define ACL_ROOT")
ENDIF()

INCLUDE_DIRECTORIES(${ACL_ROOT}/lib_acl/include)

INCLUDE_DIRECTORIES(${ACL_ROOT}/lib_acl_cpp/include)

SET(SRC_LIST 
	src/access_list.cpp
	src/http_rpc.cpp
	src/http_rpc_client.cpp
	src/http_rpc_config.cpp
	src/http_rpc_server.cpp
	src/http_rpc_servlet.cpp
	src/msg_handlers.cpp
	src/nameserver_proto_gson.cpp
	src/service_base.cpp
	src/service_register.cpp)

ADD_LIBRARY(http_rpc STATIC ${SRC_LIST})

