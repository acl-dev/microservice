## http_rpc
http_rpc is rpc framework base on [acl](https://github.com/acl-dev/acl).
with http_rpc you can rapid development of distributed system.
[acl-microservice](https://github.com/acl-dev/microservice) is base on this.


### rpc server

```cpp
class hello :public acl::service_base
{
public:
    hello(acl::http_rpc_server &server)
		:acl::service_base(server) {}

	virtual void init()
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

class hello_server: public acl::http_rpc_server
{
public:
	hello_server()
	{

	}
	virtual void init()
	{
		regist_service<hello>();
	}
};
```

```hello_server``` inherit from ```acl::http_rpc_server``` and implmemt ```init()``` and when ```hello_server```
start, ```hello_server::init()``` will be invoked by ```acl::http_rpc_server```.in this ```init()``` function, ```hello_server```
regist ```hello``` service.```hello``` public inherit ```acl::service_base``` and implememt ```init()``` function.
the  ```init()``` function will be invoked
be ```acl::http_rpc_server```. and in ```init()``` function ```hello``` do regist it's interface to ```acl::http_rpc_server```.
and it done.

when rpc_client call ```say_hello``` rpc service.hello::say_hello will be invoked by ```acl::http_rpc_server```

get complete code from [hello_server](https://github.com/acl-dev/microservice/tree/master/demo/hello_server)

### rpc client
```cpp
hello_protoc::say_hello_req req;
hello_protoc::say_hello_resp resp;
req.name = "akzi";

auto status = client.json_call("/hello_server/hello/say_hello",req, resp);
```
rpc_client is so easy to use.just ```json_call("/hello_server/hello/say_hello",req, resp);```
```/hello_server/hello/say_hello``` is service path. req is argument to send to rpc service. resp will send back from rpc service.
and it done.

get complete code from [hello_client](https://github.com/acl-dev/microservice/tree/master/demo/hello_client)
