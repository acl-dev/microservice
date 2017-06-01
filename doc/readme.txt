
acl-micorservice �ǻ���acl�����Ƶ�һ��΢�����ܡ�

http_rpc rpc���
publisher �����ư� ��������
nameservice ����ע��
gateservice api����
mqservice ��Ϣ���з���
configservice ���÷���
logservice log����
authservice ��֤����
msgservice ��Ϣ���ͷ���
monitorservice ������


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


�ʣ��ͻ�����η�����Щ����
�𣺿ͻ���ͨ��gateservice ������� ��˵ķ���

�ʣ�����֮�����ͨ�ţ�
�𣺷���֮��ͨ��http ͨѶ

�ʣ���ô�������ô��?
��ͨ��nameserver ��λ����

�ʣ����������ô�죿 
�𣺵�����Ⱥ��ʱ�򣬿�᳢ܻ�Է��������ķ�������
    ֱ����һ�������������ʣ����߶�ʧ�ܡ�



nameservice api:
	
	ע�����
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

	ɾ������
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

	��ȡ��������
	post http://ip:port/nameservice/register/find
	{
		"service_name":"/server_name/module/service"
	}

	response
	{
		"status":"ok",
		"server_addr":["127.0.0.1:10038","127.0.0.1:10039"]
	}

	��ȡ�����б�
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

	��������
	method post http://ip:port/publish/?service_name=name
	body MIME������
	�����ư�
	�����ļ�


	���������ļ���Ϣ
	method post http://ip:port/update_config?service_name=name
	body MIME������
	�����ļ�


	���߷���
	method post http://ip:port/stop?service_name=name


	ɾ������
	method post http://ip:port/delete?service_name=name






rpc:
����Э�飺http/1.1
��Ϣϵ�л�Э�� json pb 

	json:
		ʹ��IDL ��ʽ��������
		
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
		


		//���ɵĴ���

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

