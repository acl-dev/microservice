#pragma once

namespace acl
{ 
	class http_rpc_client: protected thread
	{ 
	public:
		static http_rpc_client &get_instance()
		{ 
			static http_rpc_client instance;
			return instance;
		}
		struct status_t 
		{ 
			status_t(int ret, const string &error)
				:ret_(ret),
				error_str_(error)
			{ }
			status_t()
				:ret_(0),
				error_str_("")
			{ }

			operator bool(){  return !ret_; }
			bool operator !() {  return !!ret_; }

			int ret_;
			string error_str_;
		};

		/**
		 * �Զ�ͬ��nameserver ���µ� ������Ϣ
		 * �����������߳�����ʱͬ������
		 * @param interval { int }ͬ����ʱ����,����3��
		 */
		void auto_sync_service(int interval = 3);

		/**
		* ֹͣͬ��nameserver ���µ� ������Ϣ
		*/
		void stop_sync_service();

		/**
		 * ��� nameserver
		 * @param addr { const string &} nameserver ��ַ
		 */
		void add_nameserver(const string &addr);

		/*
		* ��ӷ���
		* @param addr { const string & }�����ַ
		* @param service_path { const string & }����·��
		* @param conn_timeout { int }���ӳ�ʱʱ��
		* @param service_path { int }��д��ʱʱ��
		*/
		void add_service(const string &addr, const string &service_path, 
			int conn_timeout = 30, int rw_timeout  = 30);


		/*
		* ��ӷ���
		* @param addr { const string &} �����ַ
		* @param service_path { const std::vector<string>& } ����·������
		* @param conn_timeout { int }���ӳ�ʱʱ��
		* @param service_path { int }��д��ʱʱ��
		*/
		void add_service(const string &addr, 
			const std::vector<string> &service_paths,
			int conn_timeout = 30, int rw_timeout = 30);

		/*
		*json rpc Զ�̵���
		* @param service_name { const string& } ��������
		* @param req_type { const ReqType & } �����������
		* @param req_type { const ReqType & } ��Ӧ����
		* @return { status } ������� status == true ���ʾ���óɹ���
		*/
		template<class REQ, class RESP>
		status_t json_call(
			const string &service_name, 
			const REQ &req_type, 
			RESP &resp)
		{ 
			string buffer;

			status_t status = invoke_http_req(service_name, 
				"application/json",
				gson(req_type), 
				buffer);
			if (!status)
				return status;

			std::pair<bool, std::string> ret = gson(buffer, resp);
			if (!ret.first)
			{ 
				logger_error("gson error: %s", ret.second.c_str());
				return status_t(-1, ret.second.c_str());
			}
			return status_t();
		}
		/*
		* proto Զ�̵���
		* @param service_name { const string& } ��������
		* @param req_type { const ReqType & } �����������
		* @param req_type { const ReqType & } ��Ӧ����
		* @return { status } ������� status == true ���ʾ���óɹ���
		*/
		template<class REQ, class RESP>
		status_t proto_call(
			const string &service_name,
			const REQ &req,
			RESP &resp)
		{
			string buffer;
			std::string data = req.SerializeAsString();

			status_t status = invoke_http_req(service_name,
				"application/json",
				string(data.c_str(), data.size()),
				buffer);
			if (!status)
				return status;

			if (resp.ParseFromArray(buffer.c_str(), buffer.size()))
				return status_t();

			return status_t(-1,"ParseFromArray error");
		}

		/*
		* http Զ�̵���
		* @param service_name { const string& } ��������
		* @param req_type { const string & } �����������
		* @param req_type { const string & } ��Ӧ����
		* @return { status } ������� status == true ���ʾ���óɹ���
		*/
		status_t json_call(
			const string &service_name,
			const string &req, 
			string &resp);
	private:
		http_rpc_client();
		http_rpc_client(const http_rpc_client&);
		http_rpc_client &operator =(const http_rpc_client&);

		void init();

		status_t invoke_http_req(
			const string &service_path,
			http_request_pool *pool,
			const char *context_type,
			const string&req_data,
			string &resp_buffer);

		status_t invoke_http_req(
			const string &service_path,
			const char *context_type, 
			const string&req_data, 
			string &resp_buffer);

		bool find_connect_pool(
			const string &service_name, 
			std::vector<connect_pool*> &pools);

		bool get_connect_pool(
			const string &service_name,
			std::vector<connect_pool*> &pools);

		bool rpc_find_service_addr(
			const string &service_name, 
			std::vector<string> &addrs);
		
		void update_services_addr();

		virtual void *run();

	private:
		locker service_addrs_locker_;
		struct http_rpc_service_info
		{ 
			//{ /server/module/service}
			string service_path_;
			std::vector<string> addrs_;
			unsigned int index_;
		};
		//service_name:addr
		std::map<string, http_rpc_service_info*> service_addrs_;
		//���ӳع���
		http_request_manager *conn_manager_;

		//nameserver's services
		std::set<string> nameserver_services_;

		//sync services job interval
		int sync_inter_;

		bool start_sync_;
	};
}