#pragma once
#include "http_rpc.h"

class service_mgr : 
	public acl::service_base,
	public acl::thread
{
private:
	//service: addrs
	struct service_ttl;

	struct service_path
	{
		acl::string service_path_;
		acl::string service_;
		acl::string module_;
		acl::string server_;
	};
	struct addr_info
	{
		addr_info()
		{
			regist_time_ = 0;
			update_time_ = 0;
		}
		//server addr
		acl::string addr_;
		time_t regist_time_;
		time_t update_time_;

		std::list<service_ttl*>::iterator ttl_it_;
	};

	struct service
	{
		//interface name
		acl::string name_;
		//完整的路径/server/module/interface
		acl::string service_path_;
		//addr: addr_info
		std::map<acl::string, addr_info> addrs_;
	};

	struct module
	{
		acl::string module_name_;
		//service_name: service
		std::map<acl::string, service> services_;
	};

	struct service_ttl
	{
		service_path service_path_;
		acl::string addr_;
		long long when_;
	};

	struct server_info
	{
		std::map<acl::string, module> modules_;
	};


public:
	service_mgr(acl::http_rpc_server &server);

	~service_mgr();

private:
	//services
	bool add(const nameserver_proto::add_services_req &req,
		nameserver_proto::add_services_resp &resp);

	bool find_service(const nameserver_proto::find_service_req &req,
		nameserver_proto::find_service_resp &resp);

	bool find_services(const nameserver_proto::find_services_req &req,
		nameserver_proto::find_services_resp &resp);

	bool del_service(const nameserver_proto::del_services_req &req,
		nameserver_proto::del_services_resp &resp);

	bool list_services(const nameserver_proto::list_services_req &req,
		nameserver_proto::list_services_resp &resp);

	void init();

	void check_timeout();

	virtual void *run();

	acl::locker locker_;

	std::list<service_ttl*> service_ttl_;
	std::map<acl::string, server_info> services_;
};