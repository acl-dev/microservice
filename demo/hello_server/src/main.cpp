#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"
#include "hello_proto.h"
#include "hello_proto_gson.h"
#include "http_rpc.h"


class hello :public acl::service_base
{
public:
	hello(acl::http_rpc_server &server)
		:acl::service_base(server) {}

	void init()
	{
		server_.on_json("/hello_server/hello/say_hello", this, 
			&hello::say_hello);
	}
	bool say_hello(const hello_protoc::say_hello_req &req,
		hello_protoc::say_hello_resp &resp)
	{
		resp.str = acl::string("hello :") += req.name;
		return true;
	}
};

class hello_server : public acl::http_rpc_server
{
public:
	hello_server()
	{

	}
	void init()
	{
		regist_service<hello>();
	}
};

int main(int argc, char *argv[])
{
	acl::acl_cpp_init();

	acl::log::stdout_open(true);

	hello_server server;

	const char* addr = "127.0.0.1:10081";
#ifdef WIN32
	return server.run_alone(addr, NULL, 0, 100);
#endif
	if (argc >= 2 && strcmp(argv[1], "alone") == 0)
	{
		printf("listen on: %s\r\n", addr);

		if (argc >= 3)
		{
			// 单独运行方式
			server.run_alone(addr, argv[2], 0, 100);
		}
		else
		{
			// 单独运行方式
			server.run_alone(addr, NULL, 0, 100);
		}

		printf("Enter any key to exit now\r\n");
		getchar();
	}
	else
	{
		server.run_daemon(argc, argv);
	}
}
