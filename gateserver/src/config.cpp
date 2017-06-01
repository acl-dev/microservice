#include "http_rpc.h"
#include "config.h"

char *var_cfg_nameserver = "";
char *var_cfg_auth_key = "";

char *var_cfg_memcache_addr = "";
char *var_cfg_redis_addr = "";
int   var_cfg_conn_timeout;
int   var_cfg_rw_timeout;

acl::master_str_tbl var_conf_str_tab[] = {
	{ "var_cfg_nameserver",         "127.0.0.1:10080", &var_cfg_nameserver         },
	{ "var_cfg_memcache_addr",      "127.0.0.1:11211", &var_cfg_memcache_addr      },
	{ "var_cfg_auth_key",           "",                &var_cfg_auth_key           },
	{ "var_cfg_redis_addr",         "",                &var_cfg_redis_addr         },
	{ NULL,                         NULL,              NULL                        }
};

acl::master_bool_tbl var_conf_bool_tab[] = {
	0,0,0
};

acl::master_int_tbl var_conf_int_tab[] = {
	{ "var_cfg_conn_timeout", 30, &var_cfg_conn_timeout, 0, 0 },
	{ "var_cfg_rw_timeout",   30, &var_cfg_rw_timeout,   0, 0 }
};

acl::master_int64_tbl var_conf_int64_tab[] = {
	0, 0, 0
};