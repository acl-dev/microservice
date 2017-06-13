#pragma once
namespace acl
{
	class http_rpc_server: public master_threads
	{
	public:
		http_rpc_server();
		~http_rpc_server();

		template<class CONTEXT, class REQ, class RESP>
		bool on_json(const char *service_path,
			CONTEXT *ctx,
			bool(CONTEXT::*func)(const REQ&, RESP&))
		{

			string nameserver(http_rpc_config::var_cfg_nameserver);
			string nameserver_addr(http_rpc_config::var_cfg_nameserver_addr);
			if (nameserver.size() && nameserver_addr.size())
			{
				service_register::get_instance()
					.regist(http_rpc_config::var_cfg_server_addr, service_path);
			}

			return json_service_handles::get_instance().
				add(service_path, ctx, func);
		}

		template<class CONTEXT, class REQ, class RESP>
		bool on_pb(const char *service_path,
			CONTEXT *ctx, 
			bool(CONTEXT::*func)(const REQ&, RESP&))
		{

			string nameserver(http_rpc_config::var_cfg_nameserver);
			string nameserver_addr(http_rpc_config::var_cfg_nameserver_addr);
			if (nameserver.size() && nameserver_addr.size())
			{
				service_register::get_instance()
					.regist(http_rpc_config::var_cfg_server_addr, service_path);
			}

			return protobuf_service_handles::get_instance().
				add(service_path, ctx, func);
		}

		template<class ServiceType>
		http_rpc_server &regist_service()
		{
			services_.push_back(new ServiceType(*this));
			return *this;
		}
	protected:
		virtual void init() = 0;
	private:

		http_rpc_server(const http_rpc_server&);
		http_rpc_server &operator=(const http_rpc_server&);

		virtual void proc_on_init();


		virtual void thread_on_close(socket_stream* stream);

		virtual bool thread_on_timeout(socket_stream* stream);

		virtual bool thread_on_accept(socket_stream* stream);

		virtual bool thread_on_read(socket_stream* stream);


		session *create_session();
	private:

		std::vector<service_base*> services_;

		acl::redis_client_cluster *redis_cluster_cli_;
	};
}
