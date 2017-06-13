#include "http_rpc.h"

namespace acl
{

	http_rpc_client::http_rpc_client()
		:sync_inter_(3),
		start_sync_(false)
	{
		init();
	}

	void http_rpc_client::init()
	{
		using namespace http_rpc_config;
		//初始化nameserver service 集合
		nameserver_services_.insert(var_cfg_add_service);
		nameserver_services_.insert(var_cfg_del_service);
		nameserver_services_.insert(var_cfg_find_service);
		nameserver_services_.insert(var_cfg_find_services);
		nameserver_services_.insert(var_cfg_list_services);

		// 创建 HTTP 请求连接池集群管理对象
		conn_manager_ = new http_request_manager();
		connect_monitor* monitor =
			new connect_monitor(*conn_manager_);

		(*monitor).set_check_inter(
				http_rpc_config::var_cfg_rpc_conn_check_inter).
			set_conn_timeout(
				http_rpc_config::var_cfg_rpc_conn_check_timeout);
		// 启动后台检测线程
		conn_manager_->start_monitor(monitor);
	}
	void http_rpc_client::auto_sync_service(int interval)
	{
		if (start_sync_)
			return;
		start_sync_ = true;
		sync_inter_ = interval;
		start();
		set_detachable(true);
	}

	void http_rpc_client::stop_sync_service()
	{
		if (!start_sync_)
			return;
		start_sync_ = false;
	}

	http_rpc_client::status_t
		http_rpc_client::json_call(
			const string &service_name,
			const string &req,
			string &resp,
			unsigned int rw_timeout)
	{
		return invoke_http_req(
			service_name,
			"application/json",
			req,
			resp,
			rw_timeout);
	}
	http_rpc_client::status_t
		http_rpc_client::invoke_http_req(const string &service_name,
			const char *context_type,
			const string&req_data,
			string &resp_buffer,
			unsigned int rw_timeout)
	{

		status_t status;
		std::vector<connect_pool*> pools;

		if (!get_connect_pool(service_name, pools))
			return status_t(-1,
				string("get_connect_pool failed: ") += service_name);

		//轮循均衡 Round Robin
		//直到有一个成功，或者都是失败。
		for (size_t i = 0; i < pools.size(); i++)
		{
			status = invoke_http_req(service_name,
				(http_request_pool*)pools[i],
				context_type,
				req_data,
				resp_buffer,
				rw_timeout);

			if (status)
				break;
		}
		return status;
	}


	http_rpc_client::status_t http_rpc_client::invoke_http_req(
		const string &service_path,
		http_request_pool *pool,
		const char *context_type,
		const string&req_data,
		string &resp_buffer,
		unsigned int rw_timeout)
	{

		// 从连接池中获取一个 HTTP 连接
		connect_guard _connect_guard(*pool);
		http_request* conn = (http_request*)_connect_guard.peek();

		if (conn == NULL)
		{
			logger_error("peek connect failed from %s",
				pool->get_addr());

			return status_t(-1, string("peek connect "
				"failed from :") += pool->get_addr());
		}

		// 需要对获得的连接重置状态，
		//以清除上次请求过程的临时数据
		else
			conn->reset();

		//set rw_timeout.
		(*conn->get_client()).
			get_stream().
			set_rw_timeout(rw_timeout);
		
		//set http head
		conn->request_header().
			set_url(service_path).
			set_content_type(context_type).
			set_keep_alive(true);

		if (!conn->request(req_data.c_str(), req_data.size()))
		{
			logger_error("http_request failed");
			_connect_guard.set_keep(false);

			return status_t(-1, "http_request failed");
		}

		resp_buffer.clear();
		char  buf[8192];
		int   ret = 0;

		// 接收 HTTP 响应体数据
		while (true)
		{
			ret = conn->read_body(buf, sizeof(buf));
			if (ret == 0)
				break;
			else if (ret < 0)
			{
				_connect_guard.set_keep(false);
				logger_error("http_request read_body error");

				return status_t(-1, 
					"http_request read_body error");
			}
			resp_buffer.append(buf, ret);
		}

		if (conn->http_status() != 200)
		{
			status_t retval(conn->http_status(), resp_buffer);
			return retval;
		}

		return status_t();
	}

	bool http_rpc_client::find_connect_pool(
		const string &service_path,
		std::vector<connect_pool*> &pools)
	{

		lock_guard guard(service_addrs_locker_);

		http_rpc_service_info *info = service_addrs_[service_path];
		if (info == NULL)
		{
			info = new http_rpc_service_info();
			info->index_ = 0;
			info->service_path_ = service_path;
			service_addrs_[service_path] = info;
			return false;
		}

		for (size_t i = 0; i < info->addrs_.size(); i++)
		{
			size_t index = ++info->index_%info->addrs_.size();
			string addr = info->addrs_[index];
			connect_pool *pool = conn_manager_->get(addr.c_str());
			if (pool && pool->aliving())
				pools.push_back(pool);
		}
		return !pools.empty();
	}

	bool http_rpc_client::get_connect_pool(
		const string &service_name,
		std::vector<connect_pool*> &pools)
	{

		if (!find_connect_pool(service_name, pools))
		{
			//get service addr from nameserver
			std::vector<string> addrs;
			if (rpc_find_service_addr(service_name, addrs))
			{
				for(size_t i = 0; i < addrs.size(); i++)
					add_service(addrs[i], service_name);
				return find_connect_pool(service_name, pools);
			}
			return false;
		}
		return true;
	}

	bool http_rpc_client::rpc_find_service_addr(
		const string &service_path,
		std::vector<string> &addrs)
	{

		std::vector<connect_pool* >pools;
		if (!find_connect_pool(
			http_rpc_config::var_cfg_find_service,
			pools))
			return false;

		nameserver_proto::find_service_req req;
		nameserver_proto::find_service_resp resp;

		req.service_path = service_path;
		//发起rpc 请求 服务信息
		string buffer;
		status_t status;
		for (size_t i = 0; i < pools.size(); i++)
		{
			status = invoke_http_req(
				http_rpc_config::var_cfg_find_service,
				(http_request_pool*)pools[i],
				"application/json",
				gson(req),
				buffer,
				30);
			if (status)
				break;
		}
		if (!status)
		{
			logger_error("invoke_http_req failed: %s", 
				status.error_str_.c_str());
			return false;
		}
		std::pair<bool, std::string> ret = gson(buffer, resp);
		if (!ret.first)
		{
			logger_error("gson error: %s", ret.second.c_str());
			return  false;
		}
		if (resp.status != "ok")
		{
			logger_error("find_service resp status "
				"error: %s", resp.status.c_str());
			return false;
		}
		addrs = resp.server_addrs;
		return true;
	}

	void http_rpc_client::update_services_addr()
	{
		//req 
		nameserver_proto::find_services_req  req;
		nameserver_proto::find_services_resp resp;
		std::vector<connect_pool* >			 pools;

		//pack requst data
		do
		{
			//lock
			lock_guard guard(service_addrs_locker_);

			if (service_addrs_.empty())
				return;

			std::map<string, http_rpc_service_info*>::iterator 
				it= service_addrs_.begin();

			for (; it != service_addrs_.end(); ++it)
			{
				req.service_paths.push_back(it->first);
			}
		} while (false);

		if (req.service_paths.empty())
			return;
		
		//find connect pool for http_requst

		if (!find_connect_pool(
			http_rpc_config::var_cfg_find_services,
			pools))
		{
			logger_error("find_connect_pool failed: %s",
				http_rpc_config::var_cfg_find_services);
			return;
		}

		//发起rpc 请求 服务信息
		string buffer;
		status_t status;
		for (size_t i = 0; i < pools.size(); i++)
		{
			status = invoke_http_req(
				http_rpc_config::var_cfg_find_services,
				(http_request_pool*)pools[i],
				"application/json",
				gson(req),
				buffer,
				30);
			if (status)
				break;
		}

		//http request failed
		if (!status)
		{
			logger_error("http request failed: %s",
				status.error_str_.c_str());
			return;
		}
			
		//resp json --> resp obj
		std::pair<bool, std::string> ret = gson(buffer, resp);
		if (!ret.first)
		{
			logger_error("gson error:%s", buffer.c_str());
			return;
		}
		if (resp.status != "ok")
		{
			logger_error("resp status error: %s", resp.status.c_str());
			return;
		}
		//process resp
		//todo 优化
		lock_guard guard(service_addrs_locker_);

		for (size_t i = 0; i < req.service_paths.size(); i++)
		{
			string &service_path = req.service_paths[i];

			nameserver_proto::service_info &service_info 
				= resp.service_infos[service_path];

			std::vector<string> &addrs = service_addrs_[service_path]->addrs_;

			if (http_rpc_config::var_cfg_sync_del_service_addr)
			{
				
				// delete dead service addrs
				for (std::vector<string>::iterator it = addrs.begin();
					it != addrs.end(); )
				{
					bool not_nameserver_services = 
						nameserver_services_.find(service_path) == 
							nameserver_services_.end();

					if(http_rpc_config::var_cfg_sync_del_nameserver_service || 
						not_nameserver_services)
					{
						if (service_info.server_addrs.find(*it) == 
							service_info.server_addrs.end())
						{
							logger("delete service:%s addr:%s", 
								service_path.c_str(), it->c_str());

							conn_manager_->remove(*it);
							it = addrs.erase(it);
							continue;
						}
					}
					++it;
				}
			}

			//add new addr for service
			for (std::set<string>::iterator 
				it = service_info.server_addrs.begin(); 
				it != service_info.server_addrs.end(); 
				it++)
			{
				bool find = false;
				for (size_t i = 0; i < addrs.size(); i++)
				{
					if (addrs[i] == *it)
					{
						find = true;
						break;
					}
				}
				if (!find && it->size())
				{
					addrs.push_back(*it);
					conn_manager_->set(it->c_str(), 0);

					logger("add service:%s addr:%s",
						service_path.c_str(),it->c_str());
				}
			}
		}
	}

	void http_rpc_client::add_service( 
		const string &addr,
		const std::vector<string> &service_paths,
		int conn_timeout /* = 30 */,
		int rw_timeout /* = 30 */)
	{
		for (size_t i = 0; i < service_paths.size(); i++)
		{
			add_service(addr, service_paths[i], conn_timeout, rw_timeout);
		}
	}
	void http_rpc_client::add_nameserver(const string &addr)
	{
		std::set<string>::iterator it = nameserver_services_.begin();
		for (; it != nameserver_services_.end(); it++)
		{
			add_service(addr,*it);
		}
	}
	void http_rpc_client::add_service(
		const string &addr,
		const string &service_name, 
		int conn_timeout /* = 30 */, 
		int rw_timeout /* = 30 */)
	{
		lock_guard guard(service_addrs_locker_);

		http_rpc_service_info * service_addr = NULL;

		std::map<string, http_rpc_service_info*>::iterator itr =
			service_addrs_.find(service_name);
		if (itr == service_addrs_.end())
		{
			service_addr = new http_rpc_service_info;
			service_addr->addrs_.push_back(addr);
			service_addr->index_ = 0;
			service_addr->service_path_ = service_name;
			service_addrs_[service_name] = service_addr;
			conn_manager_->set(addr, 0, conn_timeout, rw_timeout);
			return;
		}
		service_addr = itr->second;

		for (size_t i = 0; i < service_addr->addrs_.size(); i++)
		{
			if (service_addr->addrs_[i] == addr)
			{
				return;
			}
		}
		conn_manager_->set(addr, 0);
		service_addr->addrs_.push_back(addr);
	}


	void *http_rpc_client::run()
	{
		do
		{
			timeval start, end;
			gettimeofday(&start, NULL);

			update_services_addr();

			gettimeofday(&end, NULL);

			long mills = (end.tv_sec - start.tv_sec) * 1000;
			mills += (end.tv_usec - start.tv_usec) / 1000;

			mills = sync_inter_ * 1000 - mills;

			//protect for too busy
			if (mills <= 0)
				mills = 1000;

			acl_doze(mills);

		} while (start_sync_);

		return NULL;
	}
}