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
		 * 自动同步nameserver 最新的 服务信息
		 * 函数会启动线程作定时同步任务
		 * @param interval { int }同步的时间间隔,建议3秒
		 */
		void auto_sync_service(int interval = 3);

		/**
		* 停止同步nameserver 最新的 服务信息
		*/
		void stop_sync_service();

		/**
		 * 添加 nameserver
		 * @param addr { const string &} nameserver 地址
		 */
		void add_nameserver(const string &addr);

		/*
		* 添加服务
		* @param addr { const string & }服务地址
		* @param service_path { const string & }服务路径
		* @param conn_timeout { int }连接超时时间
		* @param service_path { int }读写超时时间
		*/
		void add_service(const string &addr, const string &service_path, 
			int conn_timeout = 30, int rw_timeout  = 30);


		/*
		* 添加服务
		* @param addr { const string &} 服务地址
		* @param service_path { const std::vector<string>& } 服务路径数组
		* @param conn_timeout { int }连接超时时间
		* @param service_path { int }读写超时时间
		*/
		void add_service(const string &addr, 
			const std::vector<string> &service_paths,
			int conn_timeout = 30, int rw_timeout = 30);

		/*
		*json rpc 远程调用
		* @param service_name { const string& } 服务名称
		* @param req_type { const ReqType & } 请求参数对象
		* @param req_type { const ReqType & } 响应对象
		* @return { status } 如果返回 status == true 则表示调用成功。
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
		* proto 远程调用
		* @param service_name { const string& } 服务名称
		* @param req_type { const ReqType & } 请求参数对象
		* @param req_type { const ReqType & } 响应对象
		* @return { status } 如果返回 status == true 则表示调用成功。
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
		* http 远程调用
		* @param service_name { const string& } 服务名称
		* @param req_type { const string & } 请求参数对象
		* @param req_type { const string & } 响应对象
		* @return { status } 如果返回 status == true 则表示调用成功。
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
		//连接池管理
		http_request_manager *conn_manager_;

		//nameserver's services
		std::set<string> nameserver_services_;

		//sync services job interval
		int sync_inter_;

		bool start_sync_;
	};
}