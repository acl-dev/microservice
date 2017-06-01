#pragma once
#include <set>
namespace acl
{
	/**
	 * service_register ��nameservice��rpc_client;
	 * regist ע�����nameserver ��ȥ���������ķ����ѯ
	 * del ɾ���Ѿ�ע��ķ���
	 * stop ����ֹͣ����
	 * nameserver ��һ����ʱ�Ļ��ƣ�����̶�ʱ���ڣ�
	 * ����û�м���ע�ᣬ����ɾ����
	 */
	class service_register: public acl::thread
	{
	public:
		static service_register& get_instance();

		~service_register() 
		{

		}
		/*
		 * ע�����nameserver��ȥ
		 * @param addr {const string &}����������ַ
		 * @param service_path {const string &}��������,
		 * ��������Ϊ/{server}/{module}/{interface}
		 * @return {void}
		 */
		void regist(const string &addr, const string& service_path);

		/*
		 * ȡ��ע�����
		 * @param addr {const string &}����������ַ
		 * @param service_name {const string &}��������,
		 * ��������Ϊ/{server}/{module}/{interface}
		 * @return {void}
		 */
		void del(const string &addr, const string& service_path);

		/*
		 * ֹͣע���������
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