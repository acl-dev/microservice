#include "http_rpc.h"

namespace acl
{
	namespace http_rpc_config
	{

		int var_cfg_redis_conn_timeout = 5;
		int var_cfg_redis_rw_timeout =30;
		int var_cfg_rpc_conn_check_inter = 1;
		int var_cfg_rpc_conn_check_timeout = 5;
		int var_cfg_regist_service_inter = 3;
		int var_cfg_update_service_inter = 3;
		int var_cfg_rw_timeout = 30;

		char *var_cfg_redis_addr;
		char *var_cfg_memcache_addr = (char*)"127.0.0.1:11211";
		char *var_cfg_allow_clients = (char*)"all";
		char *var_cfg_find_service = (char*)"/nameserver/service_mgr/find";
		char *var_cfg_find_services = (char*)"/nameserver/service_mgr/finds";
		char *var_cfg_add_service = (char*)"/nameserver/service_mgr/add";
		char *var_cfg_del_service = (char*)"/nameserver/service_mgr/del";
		char *var_cfg_list_services = (char*)"/nameserver/service_mgr/list";
		char *var_cfg_server_addr;
		char *var_cfg_server_name;
		char *var_cfg_nameserver = (char*)"servername";
		char *var_cfg_nameserver_addr = (char*)"127.0.0.1:10080";

		int var_cfg_sync_del_service_addr = 1;
		int var_cfg_sync_del_nameserver_service = 0;
		int var_cfg_auto_sync_services = 1;
		int var_cfg_auto_regist_services = 1;
	}
}
