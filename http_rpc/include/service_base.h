#pragma once
namespace acl
{
	class http_rpc_server;
	class service_base
	{
	public:
		service_base(http_rpc_server &server)
			:server_(server)
		{}

		virtual ~service_base();

		virtual void init() = 0;

	protected:

		static HttpServletRequest &getHttpServletRequest();

		http_rpc_server &server_;
	};
}

