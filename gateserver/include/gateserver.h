#pragma once

class gateserver :
	public acl::master_threads
{
public:
	gateserver();
	~gateserver();
private:
	/**
	* ���麯������ĳ���ͻ������������ݿɶ���رջ����ʱ���ô˺���
	* @param stream {socket_stream*}
	* @return {bool} ���� false ���ʾ���������غ���Ҫ�ر����ӣ�
	*  �����ʾ��Ҫ���ֳ����ӣ��������������Ӧ��Ӧ�÷��� false
	*/
	virtual bool thread_on_read(acl::socket_stream* stream);

	/**
	* ���̳߳��е�ĳ���̻߳��һ������ʱ�Ļص�������
	* ���������һЩ��ʼ������
	* @param stream {socket_stream*}
	* @return {bool} ������� false ���ʾ����Ҫ��ر����ӣ�����
	*  �ؽ��������ٴ����� thread_main ����
	*/
	virtual bool thread_on_accept(acl::socket_stream* stream);

	/**
	* ��ĳ���������ӵ� IO ��д��ʱʱ�Ļص�����������ú������� true ���ʾ�����ȴ���һ��
	* ��д��������ϣ���رո�����
	* @param stream {socket_stream*}
	* @return {bool} ������� false ���ʾ����Ҫ��ر����ӣ�����
	*  �ؽ��������ٴ����� thread_main ����
	*/
	virtual bool thread_on_timeout(acl::socket_stream* stream);

	/**
	* ����ĳ���̰߳󶨵����ӹر�ʱ�Ļص�����
	* @param stream {socket_stream*}
	*/
	virtual void thread_on_close(acl::socket_stream* stream);

	/**
	* ���̳߳���һ�����̱߳�����ʱ�Ļص�����
	*/
	virtual void thread_on_init();

	/**
	* ���̳߳���һ���߳��˳�ʱ�Ļص�����
	*/
	virtual void thread_on_exit();

	/**
	* �������л��û���ݺ���õĻص��������˺���������ʱ������
	* ��Ȩ��Ϊ��ͨ���޼���
	*/
	virtual void proc_on_init();

	/**
	* �������˳�ǰ���õĻص�����
	*/
	virtual void proc_on_exit();

	acl::session *create_session();

private:

	acl::string redis_addr_;

	acl::string memcache_addr_;

	acl::redis_client_cluster *redis_cluster_cli_;
};