#include "http_rpc.h"
#include "nameserver.h"


const char* addr = "0.0.0.0:10080";

int main(int argc, char *argv[])
{
	
	acl::acl_cpp_init();
	
	acl::log::stdout_open(true);

	nameserver server;

#ifdef WIN32
	return server.run_alone(addr, NULL, 0, 100);
#endif

	if (argc >= 2 && strcmp(argv[1], "alone") == 0)
	{
		printf("listen on: %s\r\n", addr);
		if (argc >= 3)
		{
			server.run_alone(addr, argv[2], 0, 100);
		}
		else
		{
			server.run_alone(addr, NULL, 0, 100);
		}
		printf("Enter any key to exit now\r\n");
		getchar();
	}else
		server.run_daemon(argc, argv);
	return 0;
}