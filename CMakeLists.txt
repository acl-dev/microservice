project(microservice)
cmake_minimum_required(VERSION 2.8)

if(NOT ACL_ROOT)
	message(FATAL_ERROR "microservice depend on acl.  to build microservice. "
                            "U need to define ACL_ROOT,Eg: cmake .. -DAC_ROOT=/home/akzi/acl")
endif()

add_subdirectory(http_rpc)
add_subdirectory(nameserver)
add_subdirectory(demo)
