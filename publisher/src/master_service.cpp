#pragma once
#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "master_service.h"

acl::master_service::master_service()
{

}

bool acl::master_service::thread_on_read(acl::socket_stream* stream)
{
	return false;

}

bool acl::master_service::thread_on_accept(acl::socket_stream* stream)
{
	return false;

}

bool acl::master_service::thread_on_timeout(acl::socket_stream* stream)
{
	return false;

}

void acl::master_service::thread_on_close(acl::socket_stream* stream)
{

}

void acl::master_service::thread_on_init()
{

}

void acl::master_service::thread_on_exit()
{

}

bool acl::master_service::proc_exit_timer(size_t nclients, size_t nthreads)
{
	return false;

}

void acl::master_service::proc_on_init()
{
}

void acl::master_service::proc_on_exit()
{

}
