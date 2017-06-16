#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "hello_proto.h"
#include "hello_proto_gson.h"
#include "http_rpc.h"

const char *nameserver_addr = "127.0.0.1:10080";

int main()
{
	acl::acl_cpp_init();
	acl::log::stdout_open(true);

	acl::http_rpc_client &client = acl::http_rpc_client::get_instance();

	client.add_nameserver(nameserver_addr);

	hello_protoc::say_hello_req req;
	hello_protoc::say_hello_resp resp;

	req.name = "akzi";

	acl::http_rpc_client::status_t status =
		client.json_call("/hello_server/hello/say_hello",req, resp);

	if (!status)
	{
		logger("json_call error: %s",status.error_str_.c_str());
	}
	else 
		logger("json_call ok. resp:%s",resp.str.c_str()); 

	getchar();
	return 0;
}
