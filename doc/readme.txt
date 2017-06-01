
acl-micorservice 是基于acl框架设计的一套微服务框架。

http_rpc rpc框架
publisher 二进制包 发布服务。
nameservice 服务注册
gateservice api网关
mqservice 消息队列服务
configservice 配置服务
logservice log服务
authservice 认证服务
msgservice 消息推送服务
monitorservice 服务监控


                                  |------------>logic_sever----|
                                  |------------>logic_sever----|
PC-------------->|                |------------>logic_sever----|
                 |->gateserver *N |------------>logic_sever----|
android/ios----->|       |        |------------>logic_sever----|
                         |        |------------>logic_sever----|
                         |                                     | 
						 |--------------|----------|-------------
						                |          |
                                    nameserver     |
									           logservice


问：客户端如何访问这些服务？
答：客户端通过gateservice 服务访问 后端的服务

问：服务之间如何通信？
答：服务之间通过http 通讯

问：这么多服务，怎么找?
答：通过nameserver 定位服务

问：服务挂了怎么办？ 
答：当部署集群的时候，框架会尝试访问其他的服务器，
    直到有一个可以正常访问，或者都失败。



nameservice api:
	
	注册服务
	post http://ip:port/nameservice/register/add
	{
		"service_names":[
			"/server_name/module/service1",
			"/server_name/module/service2"
		],
		"server_addr":"127.0.0.1:10038"
	}

	response
	{
		"result":"ok"
	}

	删除服务
	post http://ip:port/nameservice/register/del

	{
		"service_names":[
				"/server_name/module/service1",
				"/server_name/module/service2"
			],
		"server_addr":"127.0.0.1:10038"
	}
	response
	{
		"result":"ok"
	}

	获取单个服务
	post http://ip:port/nameservice/register/find
	{
		"service_name":"/server_name/module/service"
	}

	response
	{
		"status":"ok",
		"server_addr":["127.0.0.1:10038","127.0.0.1:10039"]
	}

	获取服务列表
	get http://ip:port/nameservice/register/list

	response
	{
		"services":[
			{
				"service_name":/server_name/module/service",
				"server_addrs":["127.0.0.1:10038","127.0.0.1:10039"]
			}
		]
	}

publisher api:

	发布服务
	method post http://ip:port/publish/?service_name=name
	body MIME数据流
	二进制包
	配置文件


	更新配置文件信息
	method post http://ip:port/update_config?service_name=name
	body MIME数据流
	配置文件


	下线服务
	method post http://ip:port/stop?service_name=name


	删除服务
	method post http://ip:port/delete?service_name=name






rpc:
传输协议：http/1.1
消息系列化协议 json pb 

	json:
		使用IDL 方式生产代码
		
		//json struct
		struct req
		{
			int a;
			int b;
		};
		//json struct
		struct resp
		{
			int result;
		}

		//interface struct

		//Gson@interface
		namespace myserver
		{
			struct compu_service
			{
				virtual void add(const req &, resp &) = 0;
				virtual void sub(const req &, resp &) = 0;
			}
		}
		


		//生成的代码

		//service
		class compu_service_impl :public compu_service
		{
		public:
			compu_service()
			{
			}
		private:
			virtual void add(const req &_req, resp &_resp)
			{
				
			}
			virtual void sub(const req &_req, resp &_resp)
			{
				
			}
		}

		int main(int argc, char *argv[])
		{
			myserver server;
			server.regist<compu_service_impl>();

			server.run(argc, argv);
		}
		//client

		http_rpc_client client;
		req _req;
		_req.a = 1;
		_req.b = 2;
		resp _resp;
		
		//service path: {server}/{module}/{method}
		int ret = client.call("myserver/compu/add", _req,_resp);

