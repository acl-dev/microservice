#pragma once
namespace acl
{
    /**
	 * ����������Ļ���
	 */
	class service_handles
	{
	public:
		service_handles();

		virtual ~service_handles() {}

		/**
		 * ��ֺ����Ƿ����
		 * @param func_name{const char *} ��������
		 */
		bool exist(const char *func_name);
		
		/**
		 * ��ȡ�������
		 * @param name{const string &} ��������
		 * @return {func_handler*} 
		 * ���ڷ���func_handler *������NULL
		 */
		service_handle *get_handle(const string &name);
	protected:
		
		acl::locker lock_;
		std::map<acl::string, service_handle *> message_handles_;
	};

}