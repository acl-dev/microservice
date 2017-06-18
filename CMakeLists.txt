project(microservice)

cmake_minimum_required(VERSION 2.8)


if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "please into build dir to build!")
endif()

if(NOT ACL_ROOT)
	message(FATAL_ERROR "microservice depend on acl.  to build microservice. "
                            "U need to define ACL_ROOT,Eg: cmake .. -DAC_ROOT=/home/akzi/acl")
endif()

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "update cmake type")
	SET(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "cmake type: ${CMAKE_BUILD_TYPE}")

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
		add_definitions("-O3")
	else()
		add_definitions("-g")
endif()



add_subdirectory(http_rpc)
add_subdirectory(nameserver)
add_subdirectory(demo)
