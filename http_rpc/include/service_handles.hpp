#pragma once
namespace acl
{
    /**
	 * 服务句柄管理的基类
	 */
	class service_handles
	{
	public:
		service_handles();

		virtual ~service_handles() {}

		/**
		 * 坚持函数是否存在
		 * @param func_name{const char *} 函数名称
		 */
		bool exist(const char *func_name);
		
		/**
		 * 获取函数句柄
		 * @param name{const string &} 函数名称
		 * @return {func_handler*} 
		 * 存在返回func_handler *，否则NULL
		 */
		service_handle *get_handle(const string &name);
	protected:
		
		acl::locker lock_;
		std::map<acl::string, service_handle *> message_handles_;
	};

}