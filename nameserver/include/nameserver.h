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
	 * ��ʼ�� ����
	 */
	void init()
	{
		//ע�����
		regist_service<service_mgr>();
	}
};