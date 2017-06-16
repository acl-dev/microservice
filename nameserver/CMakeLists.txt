
project(nameserver)

cmake_minimum_required(VERSION 2.8)

if(NOT ACL_ROOT)
	message(FATAL_ERROR "need define ACL_ROOT to build it")
endif()

include_directories(
	${nameserver_SOURCE_DIR}/include
	${http_rpc_SOURCE_DIR}/include
	${ACL_ROOT}/lib_acl_cpp/include
	${ACL_ROOT}/lib_acl/include	
	)

aux_source_directory(${nameserver_SOURCE_DIR}/src SRC_LIST)

add_executable(nameserver ${SRC_LIST})

target_link_libraries(nameserver
	http_rpc
	${ACL_ROOT}/lib_acl_cpp/lib/lib_acl_cpp.a
	${ACL_ROOT}/lib_acl/lib/lib_acl.a
	${ACL_ROOT}/lib_protocol/lib/lib_protocol.a
	pthread
	z)

