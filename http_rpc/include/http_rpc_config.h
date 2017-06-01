#pragma once
namespace acl
{
	namespace http_rpc_config
	{
		//http_rpc 配置表单
		extern master_int_tbl var_conf_int_tab[];
		extern master_int64_tbl  var_conf_int64_tab[];
		extern master_str_tbl  var_conf_str_tab[];
		extern master_bool_tbl  var_conf_bool_tab[];

		//本服务器的名称
		//默认：空 需要配置好
		extern char *var_cfg_server_name;

		//nameserver的名称
		//默认：nameserver
		extern char *var_cfg_nameserver;

		//nameserver 地址
		//需要自己配置
		extern char *var_cfg_nameserver_addr;

		//server 地址
		//这个地址需要放到 nameserver 上面去，被其他的service 可见。
		//格式 ip:port 
		extern char *var_cfg_server_addr;

		//redis 集群地址
		extern char *var_cfg_redis_addr;

		//redis 连接超时时间 秒
		//连接超时 5秒
		extern int var_cfg_redis_conn_timeout;

		//redis 读写超时时间 秒
		//默认30秒
		extern int var_cfg_redis_rw_timeout;

		//memcache 服务地址
		//默认是 127.0.0.1:11211
		extern char *var_cfg_memcache_addr;

		//白名单 all 为任意client 都可以访问
		//默认是all
		extern char *var_cfg_allow_clients;

		//rpc socket 读写超时时间 秒
		//默认 30秒
		//需要加上 对方处理请求的时间
		extern int var_cfg_rw_timeout;

		//namserver 服务 查询服务的接口名字
		//默认是 /nameserver/service_mgr/find 不需要修改
		extern char * var_cfg_find_service;

		//namserver 服务 查询服务的接口名字
		//默认是 /nameserver/service_mgr/finds 不需要修改
		extern char * var_cfg_find_services;

		//namserver 服务 添加服务的接口名字
		//默认是 /nameserver/service_mgr/add 不需要修改
		extern char *var_cfg_add_service;

		//namserver 服务 删除服务的接口名字
		//默认是 /nameserver/service_mgr/del 不需要修改
		extern char *var_cfg_del_service;

		//namserver 服务 删除服务的接口名字
		//默认是 /nameserver/service_mgr/list 不需要修改
		extern char *var_cfg_list_services;

		//http_rpc 连接池 坚持连接健康的间隔
		//建议是 1 秒
		extern int var_cfg_rpc_conn_check_inter;

		//http_rpc 连接池 坚持连接健康的超时时间
		//建议是 5秒
		extern int var_cfg_rpc_conn_check_timeout;

		//注册服务 心跳的间隔
		//建议是 3秒钟
		extern int var_cfg_regist_service_inter;

		//更新service address 时间间隔
		//建议是 3秒钟
		extern int var_cfg_update_service_inter;

		//true 本地的service 地址 缓存会和nameserver上面的 同步删除
		//也就是说，如果nameserver 上面删除了某个的service的地址 本地就删除
		//false 本地的service 地址 只会同步 nameserver 新添加的地址
		//不会删除 nameserver 不存在的地址
		//默认是 true
		extern int var_cfg_sync_del_service_addr;

		//true 本地的nameserver 地址 缓存会和nameserver上面的 同步删除
		//也就是说，如果nameserver 上面删除了某个的nameserver 的地址 本地就删除
		//false 本地的nameserver 地址 只会同步 nameserver 新添加的地址
		//不会删除 nameserver 不存在的地址
		//默认是 false
		//建议 false，防止与 nameserver 失连
		extern int var_cfg_sync_del_nameserver_service;
		//默认配置
		//用户需要copy 这个初始化的代码，添加修改成自己需要的配置
		//var_cfg_server_name，
		//var_cfg_server_addr,
		//var_cfg_nameserver_addr
		//必须配置
		//实现自己的配置信息
		/*
		using acl::http_rpc_config;

		//最后一行必须是NULL,0 ,NULL,0, 0表示结束
		acl::master_int_tbl acl::http_rpc_config::var_conf_int_tab [] = {
			{ "var_cfg_redis_conn_timeout",     30, &var_cfg_redis_conn_timeout,     0, 0 },
			{ "var_cfg_redis_rw_timeout",       30, &var_cfg_redis_rw_timeout,       0, 0 },
			{ "var_cfg_rpc_conn_check_inter",   1,  &var_cfg_rpc_conn_check_inter,   0, 0 },
			{ "var_cfg_rpc_conn_check_timeout", 5,  &var_cfg_rpc_conn_check_timeout, 0, 0 },
			{ "var_cfg_register_service_inter", 3,  &var_cfg_regist_service_inter ,  0, 0 },
			{ "var_cfg_update_service_inter",   3,  &var_cfg_update_service_inter ,  0, 0 },
			{ NULL,                             0 , NULL ,                           0, 0 }
		};


		//最后一行必须是NULL,NULL,NULL 表示结束
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

		//最后一行必须是0,0,0,0,0表示结束
		acl::master_int64_tbl acl::http_rpc_config::var_conf_int64_tab [] = {

			{ 0, 0 , 0 , 0, 0 }
		};

		//最后一行必须是0,0,0表示结束
		acl::master_bool_tbl acl::http_rpc_config::var_conf_bool_tab [] = {
			{"var_cfg_sync_del_service_addr", true,   &var_cfg_sync_del_service_addr}
			{ NULL                          , false , NULL }
		};
		*/
	}

}