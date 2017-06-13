#pragma once
namespace acl
{
	struct service_handle
	{
		virtual bool invoke(const string &req, string &resp) = 0;
	};
}