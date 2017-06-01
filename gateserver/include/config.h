#pragma once

extern char *var_cfg_nameserver;
extern char *var_cfg_check_auth_service;
extern char *var_cfg_auth_key;

extern char *var_cfg_memcache_addr;
extern char *var_cfg_redis_addr;
extern int   var_cfg_conn_timeout;
extern int   var_cfg_rw_timeout;

extern acl::master_str_tbl var_conf_str_tab[];
extern acl::master_bool_tbl var_conf_bool_tab[];
extern acl::master_int_tbl var_conf_int_tab[];
extern acl::master_int64_tbl var_conf_int64_tab[];