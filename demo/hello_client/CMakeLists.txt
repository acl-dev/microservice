

project(hello_client)

cmake_minimum_required(VERSION 2.8)

if(NOT ACL_ROOT)
	message(FATAL_ERROR "need define ACL_ROOT")
endif()

include_directories(
	include
	${ACL_ROOT}/lib_acl/include
	${ACL_ROOT}/lib_acl_cpp/include
	${http_rpc_SOURCE_DIR}/include)

aux_source_directory(${hello_client_SOURCE_DIR}/src SRC_LIST)

add_executable(hello_client ${SRC_LIST})
target_link_libraries(hello_client 
	http_rpc
	${ACL_ROOT}/lib_acl_cpp/lib/lib_acl_cpp.a
	${ACL_ROOT}/lib_protocol/lib/libprotocol.a
	${ACL_ROOT}/lib_acl/lib/lib_acl.a
	pthread
	z)

