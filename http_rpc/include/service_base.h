#pragma once
namespace acl
{
	/**
	 *	所以的service都要继承这个类，实现init方法。
	 *	init 可以用来初始化一些资源，这个是函数是线程安全的。
	 *	这个架构是基于acl::master_threads 模型，提供服务接口会可能被多线程同时访问。
	 *	也就说 用户自己的数据 需要自己作多线程安全处理。
	 */
	class http_rpc_server;
	class service_base
	{
	public:
		service_base(http_rpc_server &server)
			:server_(server)
		{}

		virtual ~service_base();

		/**
		 * service 初始化函数，这个函数被http_rpc_server 回调
		 */
		virtual void init() = 0;

	protected:

		/**
		 *  获取 HttpServletReques 对象，
		 *  注意：这个函数只有服务被调用过程中才能有效，
		 *  调用前后 HttpServletRequest 是不存在的
		 */
		static HttpServletRequest &getHttpServletRequest();

		http_rpc_server &server_;
	};
}

