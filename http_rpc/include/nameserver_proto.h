#pragma once
#include <vector>
#include <string>
#include <set>
namespace nameserver_proto
{
	struct service_info
	{
		acl::string service_path;
		//Gson@optional
		std::set<acl::string> server_addrs;
	};

	/*
	json:
	{
		"service_paths":[
			"/server_name/module/service1",
			"/server_name/module/service2"
		],
		"server_addr":"127.0.0.1:10038"
	}
	{
		"result":"ok"
	}
	*/
	struct  add_services_req
	{
		std::vector<acl::string> service_paths;
		acl::string server_addr;
	};

	struct add_services_resp
	{
		acl::string result;
	};


	/*
	json:
	{
		"service_paths":[
			"/server_name/module/service1",
			"/server_name/module/service2"
		],
		"server_addr":"127.0.0.1:10038"
	}
	response
	{
	"result":"ok"
	}
	*/
	struct del_services_req
	{
		std::vector<acl::string> service_paths;
		acl::string server_addr;
	};

	struct del_services_resp
	{
		acl::string result;
	};


	/*
	json:
	{
		"service_path":"/server_name/module/service"
	}
	
	{
		"status":"ok",
		"server_addrs":["127.0.0.1:10038","127.0.0.1:10039"]
	}
	*/

	struct find_service_req
	{
		acl::string service_path;
	};

	struct find_service_resp
	{
		acl::string status;
		//Gson@optional
		std::vector<acl::string> server_addrs;
	};
	/*
	json:
	{
		"service_paths":["b","a"]
	}

	{
		"service_infos":{
			"service_name":{
				"service_name":"service_name",
				"server_addrs":["120.0.0.1:9001","127.0.0.1:9002"]
			}
		}
	}
	*/
	struct find_services_req
	{
		std::vector<acl::string> service_paths;
	};

	struct find_services_resp
	{
		acl::string status;
		//Gson@optional
		std::map<acl::string, service_info> service_infos;
	};

	/*
	json:
	{
		"services":[
			{
				"service_path":/server_name/module/service",
				"server_addrs":["127.0.0.1:10038","127.0.0.1:10039"]
			}
		]
	}
	*/

	struct list_services_req
	{
		acl::string service_path;
	};
	
	struct list_services_resp
	{
		acl::string status;
		//Gson@optional
		std::vector<service_info> services;
	};
}