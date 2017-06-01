#include "http_rpc.h"
#include "service_mgr.h"

extern int var_cfg_service_addr_timeout;

service_mgr::service_mgr(acl::http_rpc_server &server)
	:acl::service_base(server)
{

}
service_mgr::~service_mgr()
{

}
//格式是 /server/module/interface

static bool to_service_path(acl::string &path,
	std::vector<acl::string> &tokens)
{
	if (path.empty() || path[0] != '/' ||
		path[path.size() - 1] == '/')
	{
		return false;
	}
	tokens = path.split2("/");
	if (tokens.empty() || tokens.size() != 3)
	{
		return false;
	}
	return true;
}
bool service_mgr::add(
	const nameserver_proto::add_services_req &req,
	nameserver_proto::add_services_resp &resp)
{

	if (req.service_paths.empty())
	{
		resp.result = "service empty error";
		return true;
	}

	std::vector<service_path> service_paths;

	for (size_t i = 0; i < req.service_paths.size(); i++)
	{
		acl::string path = req.service_paths[i];
		std::vector<acl::string> tokens;
		if (!to_service_path(path, tokens))
		{
			resp.result = path += " service format error";
			return true;
		}

		service_path _service_path;
		_service_path.server_ = tokens[0];
		_service_path.module_ = tokens[1];
		_service_path.service_ = tokens[2];
		_service_path.service_path_= path;
		service_paths.push_back(_service_path);
	}

	//locker mutithread safe
	acl::lock_guard guard(locker_);

	for (size_t i = 0; i < service_paths.size(); i++)
	{
		service_path path = service_paths[i];
		
		module &module = services_[path.server_].
			modules_[path.module_];

		service &service_ = module.services_[path.service_];

		addr_info &addr = service_.addrs_[req.server_addr];

		module.module_name_ = path.module_;
		service_.name_ = path.service_;
		service_.service_path_ = path.service_path_;

		if (module.module_name_.empty())
			module.module_name_ = path.module_;
		if (service_.name_.empty())
			service_.name_ = path.service_;

		timeval now;
		gettimeofday(&now, NULL);
		long long when = now.tv_sec * 1000 +
			now.tv_usec / 1000 +
			var_cfg_service_addr_timeout * 1000;

		if (addr.regist_time_ == 0)
		{
			
			addr.regist_time_ = time(NULL);
			addr.update_time_ = time(NULL);
			addr.addr_ = req.server_addr;

			service_ttl *ttl = new service_ttl;
			ttl->addr_ = req.server_addr;
			ttl->service_path_ = path;
			//milliseconds
			ttl->when_ = when;

			addr.ttl_it_ = service_ttl_.
				insert(service_ttl_.end(), ttl);
			logger("add service: [%s] addr:[%s]",
				path.service_path_.c_str(), 
				req.server_addr.c_str());
		}
		else
		{
			service_ttl *ttl = *(addr.ttl_it_);
			acl_assert(ttl);

			service_ttl_.erase(addr.ttl_it_);

			ttl->when_ = when;
			addr.update_time_ = time(NULL);
			addr.ttl_it_ = service_ttl_.insert(service_ttl_.end(), ttl);
		}
	}
	resp.result = "ok";
	return true;
}

bool service_mgr::find_service(
	const nameserver_proto::find_service_req &req,
	nameserver_proto::find_service_resp &resp)
{
	//格式是 /server/module/interface
	acl::string name = req.service_path;
	std::vector<acl::string> tokens;
	if (!to_service_path(name, tokens))
	{
		resp.status = name += " service name error";
		return true;
	}

	service_path path;
	path.server_ = tokens[0];
	path.module_ = tokens[1];
	path.service_ = tokens[2];
	path.service_path_= name;

	acl::lock_guard guard(locker_);

	service &service_ =
		services_[path.server_]
		.modules_[path.module_].
		services_[path.service_];

	if (service_.addrs_.empty())
	{
		resp.status = "not exist";
		return true;
	}
	for (std::map<acl::string, addr_info>::iterator it
		= service_.addrs_.begin();
		it != service_.addrs_.end(); ++it)
	{
		resp.server_addrs.push_back(it->first);
	}
	resp.status = "ok";
	return true;
}

bool service_mgr::find_services(
	const nameserver_proto::find_services_req &req,
	nameserver_proto::find_services_resp &resp)
{
	if (req.service_paths.empty())
	{
		resp.status = "service names empty error";
		return true;
	}

	std::vector<service_path> service_paths;

	for (size_t i = 0; i < req.service_paths.size(); i++)
	{
		acl::string path = req.service_paths[i];
		std::vector<acl::string> tokens;
		if (!to_service_path(path, tokens))
		{
			resp.status = path += " service format error";
			return true;
		}

		service_path _service_path;
		_service_path.server_ = tokens[0];
		_service_path.module_ = tokens[1];
		_service_path.service_ = tokens[2];
		_service_path.service_path_ = path;
		service_paths.push_back(_service_path);
	}

	//locker mutithread safe
	acl::lock_guard guard(locker_);
	for (size_t i = 0; i < service_paths.size(); i++)
	{
		service_path path = service_paths[i];
		nameserver_proto::service_info &info =
			resp.service_infos[path.service_path_];

		info.service_path = path.service_path_;

		module &module =
			services_[path.server_].
			modules_[path.module_];

		service &service_ = module.services_[path.service_];
		if (module.services_.empty() || service_.addrs_.empty())
		{
			continue;
		}
		for (std::map<acl::string, addr_info>::iterator
			it = service_.addrs_.begin();
			it != service_.addrs_.end(); 
			++it)
		{
			info.server_addrs.insert(it->first);
		}
	}
	resp.status = "ok";
	return true;
}

bool service_mgr::del_service(
	const nameserver_proto::del_services_req &req,
	nameserver_proto::del_services_resp &resp)
{
	if (req.service_paths.empty())
	{
		resp.result = "service names empty error";
		return true;
	}

	std::vector<service_path> service_paths;

	for (size_t i = 0; i < req.service_paths.size(); i++)
	{
		acl::string path = req.service_paths[i];
		std::vector<acl::string> tokens;
		if (!to_service_path(path, tokens))
		{
			resp.result = path += " service format error";
			return true;
		}

		service_path _service_path;
		_service_path.server_ = tokens[0];
		_service_path.module_ = tokens[1];
		_service_path.service_ = tokens[2];
		_service_path.service_path_ = path;
		service_paths.push_back(_service_path);
	}

	//locker mutithread safe
	acl::lock_guard guard(locker_);
	for (size_t i = 0; i < service_paths.size(); i++)
	{
		service_path path = service_paths[i];

		service &service_ =
			services_[path.server_].
			modules_[path.module_].services_[path.service_];

		if (service_.addrs_.empty())
		{
			continue;
		}
		std::map<acl::string, addr_info>::iterator it =
			service_.addrs_.find(req.server_addr);
		if (it != service_.addrs_.end())
		{
			service_ttl_.erase(it->second.ttl_it_);
			service_.addrs_.erase(it);
		}
	}
	resp.result = "ok";
	return true;
}
bool service_mgr::list_services(
	const nameserver_proto::list_services_req &req,
	nameserver_proto::list_services_resp &resp)
{
	if (req.service_path.empty() || req.service_path[0] != '/')
	{
		resp.status = "path error";
		return true;
	}
	acl::string path = req.service_path;
	std::vector<acl::string> tokens = path.split2("/");

	acl::lock_guard guard(locker_);
	//list all services
	if (tokens.size() == 0)
	{
		for (std::map<acl::string, server_info>::iterator
			si_it = services_.begin();
			si_it != services_.end(); ++si_it)
		{
			server_info &sinfo = si_it->second;
			for (std::map<acl::string, module>::iterator
				mit = sinfo.modules_.begin();
				mit != sinfo.modules_.end();
				mit++)
			{
				module &module = mit->second;
				for (std::map<acl::string, service>::iterator
					sit = module.services_.begin();
					sit != module.services_.end();
					sit++)
				{
					service &serv = sit->second;

					nameserver_proto::service_info info;
					info.service_path = serv.service_path_;
					for (std::map<acl::string, addr_info>::iterator
						ait = serv.addrs_.begin();
						ait != serv.addrs_.end();
						ait++)
					{
						info.server_addrs.insert(ait->first);
					}
					if (info.server_addrs.size())
					{
						resp.services.push_back(info);
					}
						
				}
			}
		}
	}
	//list path start with "/{server}" services
	else if (tokens.size() == 1)
	{
		std::map<acl::string, module> &modules =
			services_[tokens[0]].modules_;

		for (std::map<acl::string, module>::iterator
			mit = modules.begin();
			mit != modules.end();
			mit++)
		{
			module &module = mit->second;
			for (std::map<acl::string, service>::iterator
				sit = module.services_.begin();
				sit != module.services_.end();
				sit++)
			{
				service &serv = sit->second;

				nameserver_proto::service_info info;
				info.service_path = serv.service_path_;
				for (std::map<acl::string, addr_info>::iterator
					ait = serv.addrs_.begin();
					ait != serv.addrs_.end();
					ait++)
				{
					info.server_addrs.insert(ait->first);
				}
				if (info.server_addrs.size())
					resp.services.push_back(info);
			}
		}

	}
	//list path start with "/{server}/{module}" services
	else if (tokens.size() == 2)
	{
		std::map<acl::string, service> & services = 
			services_[tokens[0]].
			modules_[tokens[1]].
			services_;

		for (std::map<acl::string, service>::iterator
			sit = services.begin();
			sit !=services.end();
			sit++)
		{
			service &serv = sit->second;
			nameserver_proto::service_info info;
			info.service_path = serv.service_path_;
			for (std::map<acl::string, addr_info>::iterator
				ait = serv.addrs_.begin();
				ait != serv.addrs_.end();
				ait++)
			{
				info.server_addrs.insert(ait->first);
			}
			if (info.server_addrs.size())
				resp.services.push_back(info);
		}

	}
	else if (tokens.size() == 3)
	{
		service &serv =
			services_[tokens[0]].
			modules_[tokens[1]].
			services_[tokens[2]];

		services_[tokens[1]];

		nameserver_proto::service_info info;
		info.service_path = req.service_path;
		for (std::map<acl::string, addr_info>::iterator
			ait = serv.addrs_.begin();
			ait != serv.addrs_.end();
			ait++)
		{
			info.server_addrs.insert(ait->first);
		}
		if (info.server_addrs.size())
			resp.services.push_back(info);
	}

	resp.status = "ok";
	return true;
}

void service_mgr::init()
{
	using namespace acl::http_rpc_config;

	acl_assert(server_.on_json(
		var_cfg_add_service, 
		this,
		&service_mgr::add));

	acl_assert(server_.on_json(
		var_cfg_find_service, this,
		&service_mgr::find_service));

	acl_assert(server_.on_json(
		var_cfg_find_services, this,
		&service_mgr::find_services));

	acl_assert(server_.on_json(
		var_cfg_del_service, this,
		&service_mgr::del_service));

	acl_assert(server_.on_json(
		var_cfg_list_services, this,
		&service_mgr::list_services));

	nameserver_proto::add_services_req req;
	nameserver_proto::add_services_resp resp;

	req.server_addr = acl::http_rpc_config::var_cfg_nameserver_addr;
	req.service_paths.push_back(var_cfg_add_service);
	req.service_paths.push_back(var_cfg_find_service);
	req.service_paths.push_back(var_cfg_find_services);
	req.service_paths.push_back(var_cfg_del_service);
	req.service_paths.push_back(var_cfg_list_services);

	add(req,resp);

	//checkout timeout thread
	start();
}

void *service_mgr::run()
{
	do
	{
		check_timeout();
		acl_doze(1000);

	} while (true);
}

void service_mgr::check_timeout()
{
	acl::lock_guard guard(locker_);

	while (service_ttl_.size())
	{
		service_ttl *ttl = service_ttl_.front();
		
		timeval now;
		gettimeofday(&now, NULL);

		long long millis = now.tv_sec * 1000 + now.tv_usec / 1000;
		if (ttl->when_ < millis)
		{
			service_path path = ttl->service_path_;
			service &service_ = 
				services_[path.server_].
				modules_[path.module_].
				services_[path.service_];

			service_ttl_.pop_front();
			if (service_.addrs_.empty())
				continue;

			logger("delete service: [%s] addr:[%s] ",
				ttl->service_path_.service_path_.c_str(),
				ttl->addr_.c_str());

			service_.addrs_.erase(ttl->addr_);

			delete ttl;
			continue;
		}
		return;
	}
}
