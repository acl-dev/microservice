#pragma once
namespace acl
{
	class service_handles
	{
	public:
		service_handles();

		virtual ~service_handles() {}

		bool exist(const char *func_name);
		
		service_handle *get_handle(const string &name);

	protected:
		
		acl::locker lock_;
		std::map<acl::string, service_handle *> message_handles_;
	};

}