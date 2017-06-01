#include "http_rpc.h"

namespace acl
{
	acl::service_handles::service_handles()
	{

	}

	bool acl::service_handles::exist(const char *func_name)
	{
		acl::lock_guard guard(lock_);
		return message_handles_.find(func_name) != message_handles_.end();
	}

	service_handle * service_handles::get_handle(const string &name)
	{
		acl::lock_guard guard(lock_);
		std::map<acl::string, service_handle *>::iterator
			itr = message_handles_.find(name);
		if (itr == message_handles_.end())
			return NULL;
		return itr->second;
	}

}
