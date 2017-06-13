#pragma once
namespace acl
{
	namespace http_rpc_config
	{
		extern master_int_tbl var_conf_int_tab[];
		extern master_int64_tbl  var_conf_int64_tab[];
		extern master_str_tbl  var_conf_str_tab[];
		extern master_bool_tbl  var_conf_bool_tab[];

		extern char *var_cfg_server_name;

		extern char *var_cfg_nameserver;

		extern char *var_cfg_nameserver_addr;

		extern char *var_cfg_server_addr;

		extern char *var_cfg_redis_addr;

		extern int var_cfg_redis_conn_timeout;

		extern int var_cfg_redis_rw_timeout;

		extern char *var_cfg_memcache_addr;

		extern char *var_cfg_allow_clients;

		extern int var_cfg_rw_timeout;

		extern char * var_cfg_find_service;

		extern char * var_cfg_find_services;

		extern char *var_cfg_add_service;

		extern char *var_cfg_del_service;

		extern char *var_cfg_list_services;

		extern int var_cfg_rpc_conn_check_inter;

		extern int var_cfg_rpc_conn_check_timeout;

		extern int var_cfg_regist_service_inter;

		extern int var_cfg_update_service_inter;

		extern int var_cfg_sync_del_service_addr;

		extern int var_cfg_sync_del_nameserver_service;

		/*
		using acl::http_rpc_config;

		acl::master_int_tbl acl::http_rpc_config::var_conf_int_tab [] = {
			{ "var_cfg_redis_conn_timeout",     30, &var_cfg_redis_conn_timeout,     0, 0 },
			{ "var_cfg_redis_rw_timeout",       30, &var_cfg_redis_rw_timeout,       0, 0 },
			{ "var_cfg_rpc_conn_check_inter",   1,  &var_cfg_rpc_conn_check_inter,   0, 0 },
			{ "var_cfg_rpc_conn_check_timeout", 5,  &var_cfg_rpc_conn_check_timeout, 0, 0 },
			{ "var_cfg_register_service_inter", 3,  &var_cfg_regist_service_inter ,  0, 0 },
			{ "var_cfg_update_service_inter",   3,  &var_cfg_update_service_inter ,  0, 0 },
			{ NULL,                             0 , NULL ,                           0, 0 }
		};

		acl::master_str_tbl acl::http_rpc_config::var_conf_str_tab [] = {

			{ "var_cfg_find_service",    "/nameserver/service_mgr/find",  &var_cfg_find_service    },
			{ "var_cfg_find_services",   "/nameserver/service_mgr/finds", &var_cfg_find_services   },
			{ "var_cfg_add_service",     "/nameserver/service_mgr/add",   &var_cfg_add_service     },
			{ "var_cfg_del_service",     "/nameserver/service_mgr/del",   &var_cfg_del_service     },
			{ "var_cfg_del_service",     "/nameserver/service_mgr/list",  &var_cfg_list_services   },
			{ "var_cfg_memcache_addr",   "127.0.0.1:11211",               &var_cfg_memcache_addr   },
			{ "var_cfg_nameserver",      "nameserver",					  &var_cfg_nameserver      },
			{ "var_cfg_nameserver_addr", "",					          &var_cfg_nameserver_addr },
			{ "var_cfg_allow_clients",   "all",                           &var_cfg_allow_clients   },
			{ "var_cfg_redis_addr",	     "",                              &var_cfg_redis_addr      },
			{ "var_cfg_server_addr",     "",                              &var_cfg_server_addr     },
			{ "var_cfg_server_name",     "",                              &var_cfg_server_name     },
			{ NULL,                      NULL,                            NULL                     }
		};

		acl::master_int64_tbl acl::http_rpc_config::var_conf_int64_tab [] = {

			{ 0, 0 , 0 , 0, 0 }
		};

		acl::master_bool_tbl acl::http_rpc_config::var_conf_bool_tab [] = {
			{"var_cfg_sync_del_service_addr", true,   &var_cfg_sync_del_service_addr}
			{ NULL                          , false , NULL }
		};
		*/
	}

}