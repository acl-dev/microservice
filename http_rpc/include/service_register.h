#pragma once
#include <set>
namespace acl
{
	/**
	 * service_register 是nameservice的rpc_client;
	 * regist 注册服务到nameserver 上去，被其他的服务查询
	 * del 删除已经注册的服务。
	 * stop 可以停止服务。
	 * nameserver 有一个超时的机制，如果固定时间内，
	 * 服务没有继续注册，将会删除。
	 */
	class service_register: public acl::thread
	{
	public:
		static service_register& get_instance();

		~service_register() 
		{

		}
		/*
		 * 注册服务到nameserver上去
		 * @param addr {const string &}本服务器地址
		 * @param service_path {const string &}服务名称,
		 * 命名规则为/{server}/{module}/{interface}
		 * @return {void}
		 */
		void regist(const string &addr, const string& service_path);

		/*
		 * 取消注册服务
		 * @param addr {const string &}本服务器地址
		 * @param service_name {const string &}服务名称,
		 * 命名规则为/{server}/{module}/{interface}
		 * @return {void}
		 */
		void del(const string &addr, const string& service_path);

		/*
		 * 停止注册服务心跳
		 */
		void stop();
	private:
		service_register();
		service_register(const service_register&);
		service_register &operator= (const service_register&);

		void rpc_regist_service(const string &addr, 
			const std::vector<string>& service_names);

		virtual void* run();

		bool is_stop_;

		locker locker_;
		//addr:service_path
		std::map<string, std::set<string>> services;
	};
}