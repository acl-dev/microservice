#pragma once
#include <set>
namespace acl
{
	class service_register: public acl::thread
	{
	public:
		static service_register& get_instance();

		~service_register() 
		{

		}

		void regist(const string &addr, const string& service_path);

		void del(const string &addr, const string& service_path);

		void stop();
	private:
		service_register();

		service_register(const service_register&);
		
		service_register &operator= (const service_register&);

		void rpc_regist_service(const string &addr, 
			const std::vector<string>& service_names);

		virtual void* run();

		void do_regist_service();
	private:


		bool is_stop_;

		locker locker_;
		//addr:service_path
		std::map<string, std::set<string> > services_;
	};
}
