#pragma once
namespace acl
{
	namespace http_rpc_config
	{
		//http_rpc ���ñ�
		extern master_int_tbl var_conf_int_tab[];
		extern master_int64_tbl  var_conf_int64_tab[];
		extern master_str_tbl  var_conf_str_tab[];
		extern master_bool_tbl  var_conf_bool_tab[];

		//��������������
		//Ĭ�ϣ��� ��Ҫ���ú�
		extern char *var_cfg_server_name;

		//nameserver������
		//Ĭ�ϣ�nameserver
		extern char *var_cfg_nameserver;

		//nameserver ��ַ
		//��Ҫ�Լ�����
		extern char *var_cfg_nameserver_addr;

		//server ��ַ
		//�����ַ��Ҫ�ŵ� nameserver ����ȥ����������service �ɼ���
		//��ʽ ip:port 
		extern char *var_cfg_server_addr;

		//redis ��Ⱥ��ַ
		extern char *var_cfg_redis_addr;

		//redis ���ӳ�ʱʱ�� ��
		//���ӳ�ʱ 5��
		extern int var_cfg_redis_conn_timeout;

		//redis ��д��ʱʱ�� ��
		//Ĭ��30��
		extern int var_cfg_redis_rw_timeout;

		//memcache �����ַ
		//Ĭ���� 127.0.0.1:11211
		extern char *var_cfg_memcache_addr;

		//������ all Ϊ����client �����Է���
		//Ĭ����all
		extern char *var_cfg_allow_clients;

		//rpc socket ��д��ʱʱ�� ��
		//Ĭ�� 30��
		//��Ҫ���� �Է����������ʱ��
		extern int var_cfg_rw_timeout;

		//namserver ���� ��ѯ����Ľӿ�����
		//Ĭ���� /nameserver/service_mgr/find ����Ҫ�޸�
		extern char * var_cfg_find_service;

		//namserver ���� ��ѯ����Ľӿ�����
		//Ĭ���� /nameserver/service_mgr/finds ����Ҫ�޸�
		extern char * var_cfg_find_services;

		//namserver ���� ��ӷ���Ľӿ�����
		//Ĭ���� /nameserver/service_mgr/add ����Ҫ�޸�
		extern char *var_cfg_add_service;

		//namserver ���� ɾ������Ľӿ�����
		//Ĭ���� /nameserver/service_mgr/del ����Ҫ�޸�
		extern char *var_cfg_del_service;

		//namserver ���� ɾ������Ľӿ�����
		//Ĭ���� /nameserver/service_mgr/list ����Ҫ�޸�
		extern char *var_cfg_list_services;

		//http_rpc ���ӳ� ������ӽ����ļ��
		//������ 1 ��
		extern int var_cfg_rpc_conn_check_inter;

		//http_rpc ���ӳ� ������ӽ����ĳ�ʱʱ��
		//������ 5��
		extern int var_cfg_rpc_conn_check_timeout;

		//ע����� �����ļ��
		//������ 3����
		extern int var_cfg_regist_service_inter;

		//����service address ʱ����
		//������ 3����
		extern int var_cfg_update_service_inter;

		//true ���ص�service ��ַ ������nameserver����� ͬ��ɾ��
		//Ҳ����˵�����nameserver ����ɾ����ĳ����service�ĵ�ַ ���ؾ�ɾ��
		//false ���ص�service ��ַ ֻ��ͬ�� nameserver ����ӵĵ�ַ
		//����ɾ�� nameserver �����ڵĵ�ַ
		//Ĭ���� true
		extern int var_cfg_sync_del_service_addr;

		//true ���ص�nameserver ��ַ ������nameserver����� ͬ��ɾ��
		//Ҳ����˵�����nameserver ����ɾ����ĳ����nameserver �ĵ�ַ ���ؾ�ɾ��
		//false ���ص�nameserver ��ַ ֻ��ͬ�� nameserver ����ӵĵ�ַ
		//����ɾ�� nameserver �����ڵĵ�ַ
		//Ĭ���� false
		//���� false����ֹ�� nameserver ʧ��
		extern int var_cfg_sync_del_nameserver_service;
		//Ĭ������
		//�û���Ҫcopy �����ʼ���Ĵ��룬����޸ĳ��Լ���Ҫ������
		//var_cfg_server_name��
		//var_cfg_server_addr,
		//var_cfg_nameserver_addr
		//��������
		//ʵ���Լ���������Ϣ
		/*
		using acl::http_rpc_config;

		//���һ�б�����NULL,0 ,NULL,0, 0��ʾ����
		acl::master_int_tbl acl::http_rpc_config::var_conf_int_tab [] = {
			{ "var_cfg_redis_conn_timeout",     30, &var_cfg_redis_conn_timeout,     0, 0 },
			{ "var_cfg_redis_rw_timeout",       30, &var_cfg_redis_rw_timeout,       0, 0 },
			{ "var_cfg_rpc_conn_check_inter",   1,  &var_cfg_rpc_conn_check_inter,   0, 0 },
			{ "var_cfg_rpc_conn_check_timeout", 5,  &var_cfg_rpc_conn_check_timeout, 0, 0 },
			{ "var_cfg_register_service_inter", 3,  &var_cfg_regist_service_inter ,  0, 0 },
			{ "var_cfg_update_service_inter",   3,  &var_cfg_update_service_inter ,  0, 0 },
			{ NULL,                             0 , NULL ,                           0, 0 }
		};


		//���һ�б�����NULL,NULL,NULL ��ʾ����
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

		//���һ�б�����0,0,0,0,0��ʾ����
		acl::master_int64_tbl acl::http_rpc_config::var_conf_int64_tab [] = {

			{ 0, 0 , 0 , 0, 0 }
		};

		//���һ�б�����0,0,0��ʾ����
		acl::master_bool_tbl acl::http_rpc_config::var_conf_bool_tab [] = {
			{"var_cfg_sync_del_service_addr", true,   &var_cfg_sync_del_service_addr}
			{ NULL                          , false , NULL }
		};
		*/
	}

}