#pragma once
namespace acl
{
	class http_rpc_servlet: public HttpServlet
	{
	public:
		http_rpc_servlet(socket_stream* stream, session* session);

		HttpServletRequest *get_httpServletRequest();

		HttpServletResponse *get_httpServletResponse();

	private:
		virtual bool doGet(HttpServletRequest&, HttpServletResponse&);
		
		virtual bool doPost(HttpServletRequest&, HttpServletResponse&);

		virtual bool doUnknown(HttpServletRequest&, HttpServletResponse&);

		virtual bool doError(HttpServletRequest&, HttpServletResponse&);

		service_handle *get_handle(const string &message_type, 
			const string &func_name);

		HttpServletRequest *req_;
		HttpServletResponse *resp_;
	};
}