#include "http_rpc.h"

namespace acl
{


	http_rpc_server::http_rpc_server()
	    :redis_cluster_cli_(NULL)
    {
		set_cfg_int(http_rpc_config::var_conf_int_tab);
		set_cfg_int64(http_rpc_config::var_conf_int64_tab);
		set_cfg_str(http_rpc_config::var_conf_str_tab);
		set_cfg_bool(http_rpc_config::var_conf_bool_tab);
	}

	http_rpc_server::~http_rpc_server()
	{
        if (redis_cluster_cli_ != NULL)
            delete redis_cluster_cli_;

		for (size_t i = 0; i < services_.size(); i++)
		{
			delete services_[i];
		}
	}

	void http_rpc_server::proc_on_init()
	{
		string redis_addr(http_rpc_config::var_cfg_redis_addr);

		if (redis_addr.size())
		{
			redis_cluster_cli_ = new acl::redis_client_cluster();
			redis_cluster_cli_->set(redis_addr, acl_var_threads_pool_limit);
		}
		string allow_clients(http_rpc_config::var_cfg_allow_clients);
		if (allow_clients.size())
		{
            logger("allow clients:%s", allow_clients.c_str());
			access_list::get_instance()
				.set_allow_clients(allow_clients);
		}

		
		http_rpc_client& client = http_rpc_client::get_instance();
		string addr(http_rpc_config::var_cfg_nameserver_addr);
		if (addr.size())
		{
			client.add_nameserver(addr);
		}
		else
			logger_warn("nameserver address empty !!!!!");

		if (http_rpc_config::var_cfg_auto_sync_services != 0)
		{
			//start sync services from nameserver
			client.auto_sync_service();
		}

		if (http_rpc_config::var_cfg_auto_regist_services != 0)
		{
			//nameserver
			service_register::get_instance().start();
		}

		//init child
		init();

		//init service
		for (std::vector<service_base*>::iterator
			iter = services_.begin();
			iter != services_.end();
			++iter)
		{
			(*iter)->init();
		}
	}

	void http_rpc_server::thread_on_close(socket_stream* stream)
	{
        acl_assert(stream);
        http_rpc_servlet *servlet =
                static_cast<http_rpc_servlet*>(stream->get_ctx());

        const char* peer = stream->get_peer(false);

		/**
		 * servlet maybe null. if in function thread_on_accept
		 * do access_list() false. and stream has create http_rpc_servlet
		 */
		if(servlet == NULL)
			return;

		session *sess = &servlet->getSession();

        delete servlet;
        if(sess != NULL)
            delete sess;

	}

	bool http_rpc_server::thread_on_timeout(socket_stream* stream)
	{
		return false;
	}

	bool http_rpc_server::thread_on_accept(socket_stream* stream)
	{
		const char* peer = stream->get_peer(false);

		if (peer == NULL || *peer == 0)
		{
			logger_warn("invalid client peer: %s"
				, peer ? peer : "null");
			return false;
		}

//		if (access_list::get_instance().check_client(peer) == false)
//		{
//			logger_warn("denied from server ip: %s", peer);
//			return false;
//		}

		//rpc 读写超时时间
		stream->set_rw_timeout(http_rpc_config::var_cfg_rw_timeout);

		http_rpc_servlet *servlet = new http_rpc_servlet;

		stream->set_ctx(servlet);

		return true;
	}

	bool http_rpc_server::thread_on_read(socket_stream* stream)
	{
        //logger("stream to read");
		http_rpc_servlet *servlet = (http_rpc_servlet *)stream->get_ctx();
		if (servlet == NULL)
			logger_fatal("servlet null!");

		session *sess = create_session();
		return servlet->doRun(*sess, stream);
	}


	session * http_rpc_server::create_session()
	{
		if (redis_cluster_cli_)
		{
			return new redis_session(*redis_cluster_cli_,
				acl_var_threads_pool_limit);
		}
		return new memcache_session(http_rpc_config::var_cfg_memcache_addr);
	}

}


