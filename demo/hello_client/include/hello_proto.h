#pragma once
namespace hello_protoc
{
	struct say_hello_req
	{
		acl::string name;
	};

	struct say_hello_resp
	{
		acl::string str;
	};
}