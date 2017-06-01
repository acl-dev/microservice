#include "http_rpc.h"

namespace acl
{
	service_register& service_register::get_instance()
	{
		static service_register inst;
		return inst;
	}

	service_register::service_register()
		:is_stop_(false)
	{

	}


	void* service_register::run()
	{
		int inter = 
			http_rpc_config::var_cfg_regist_service_inter;
		
		//wait for server init
		acl_doze(1000);

		do
		{
			timeval start, end;
			gettimeofday(&start, NULL);
			locker_.lock();
			for (std::map<string, std::set<string>>::iterator 
				it =services.begin(); 
				it != services.end(); 
				++it)
			{
				std::vector<string> services;
				for (std::set<string>::iterator 
					set_it =it->second.begin();
					set_it != it->second.end(); 
					set_it++)
				{
					services.push_back(*set_it);
				}
				if (services.size())
					rpc_regist_service(it->first, services);
			}
			locker_.unlock();
			gettimeofday(&end, NULL);

			long millis = (end.tv_sec - start.tv_sec) * 1000;
			millis += (end.tv_usec - start.tv_usec) / 1000;

			millis = inter * 1000 - millis;

			if (millis <= 0)
				millis = 1;

			acl_doze(millis);

		} while (!is_stop_);

		return NULL;
	}

	
	void service_register::rpc_regist_service(const string &addr, 
		const std::vector<string>& service_paths)
	{
		nameserver_proto::add_services_req req;
		nameserver_proto::add_services_resp resp;

		req.server_addr = addr;
		req.service_paths = service_paths;

		http_rpc_client::status_t status =
			http_rpc_client::get_instance().
			json_call(http_rpc_config::var_cfg_add_service,
				req, resp);

		if (!status)
		{
			logger_error("json_call failed:%s",
				status.error_str_.c_str());
		}
	}

	void service_register::del(const string &addr_,
		const string& service_path)
	{
		nameserver_proto::del_services_req req;
		nameserver_proto::del_services_resp resp;

		req.server_addr = addr_;
		req.service_paths.push_back(service_path);

		http_rpc_client::status_t status =
			http_rpc_client::get_instance().
			json_call(http_rpc_config::var_cfg_del_service,
				req, resp);
		if (!status)
		{
			logger_error("json_call failed:%s", status.error_str_.c_str());
		}
		//del from service success 
		//service from register timer
		//when lost heartheat.nameserver will delete timeout service

		lock_guard guard(locker_);
		services[addr_].erase(service_path);
	}

	void service_register::regist(const string &addr_,
		const string& service_path)
	{
		lock_guard guard(locker_);
		services[addr_].insert(service_path);
	}

	void service_register::stop()
	{
		is_stop_ = true;
	}

	

}