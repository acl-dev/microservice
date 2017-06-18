#pragma once
#include "http_rpc.h"
#include "service_mgr.h"

class nameserver :public acl::http_rpc_server
{
public:
	nameserver()
	{

	}
	/*
	 * 初始化 服务
	 */
	void init()
	{
		//注册服务
		regist_service<service_mgr>();
	}
};