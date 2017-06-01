#include "http_rpc.h"
#include "config.h"
#include "gateserver.h"
#include "gate_servlet.h"

gateserver::gateserver()
{
	set_cfg_bool(var_conf_bool_tab);
	set_cfg_int(var_conf_int_tab);
	set_cfg_int64(var_conf_int64_tab);
	set_cfg_str(var_conf_str_tab);

}
gateserver::~gateserver()
{
	if (redis_cluster_cli_)
		delete redis_cluster_cli_;

	acl::http_rpc_client::get_instance().stop_sync_service();
}
bool gateserver::thread_on_read(acl::socket_stream* stream)
{
	gate_servlet *servlet = (gate_servlet *)stream->get_ctx();
	acl::session *sess = create_session();

	return servlet->doRun(*sess, stream);
}

bool gateserver::thread_on_accept(acl::socket_stream* stream)
{
	gate_servlet *servlet = new gate_servlet;
	
	stream->set_ctx(servlet);
	return true;
}

bool gateserver::thread_on_timeout(acl::socket_stream* stream)
{
	return true;
}

void gateserver::thread_on_close(acl::socket_stream* stream)
{
	gate_servlet* servlet = (gate_servlet*) stream->get_ctx();
	acl::session *sess = &servlet->getSession();

	delete sess;
	delete servlet;
}

void gateserver::thread_on_init()
{

}

void gateserver::thread_on_exit()
{

}

void gateserver::proc_on_init()
{
	//redis addr
	redis_addr_ = var_cfg_redis_addr;
	memcache_addr_ = var_cfg_memcache_addr;

	//配置 http_rpc_client 
	acl::http_rpc_client &client = acl::http_rpc_client::get_instance();

	//设置nameserver
	client.add_nameserver(var_cfg_nameserver);

	//自动同步 namesever 最新的服务信息
	client.auto_sync_service();
}

void gateserver::proc_on_exit()
{

}

acl::session *gateserver::create_session()
{
	if (redis_cluster_cli_)
	{
		return new acl::redis_session(*redis_cluster_cli_,
			acl_var_threads_pool_limit);
	}
	return new acl::memcache_session(memcache_addr_);
}