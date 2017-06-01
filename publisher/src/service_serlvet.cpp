#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "service_serlvet.h"


acl::service_servlet::service_servlet()
{

}


bool acl::service_servlet::doGet(acl::HttpServletRequest&, acl::HttpServletResponse&)
{
	return true;
}

bool acl::service_servlet::doPost(acl::HttpServletRequest&, acl::HttpServletResponse&)
{
	return true;
}

bool acl::service_servlet::publish(acl::HttpServletRequest&, acl::HttpServletResponse&)
{
	return true;
}

bool acl::service_servlet::stop(acl::HttpServletRequest&, acl::HttpServletResponse&)
{
	return true;
}

bool acl::service_servlet::update_config(acl::HttpServletRequest&, acl::HttpServletResponse&)
{
	return true;
}

