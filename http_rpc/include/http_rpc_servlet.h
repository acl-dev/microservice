#pragma once
namespace acl
{
	class http_rpc_servlet: public HttpServlet
	{
	public:
		http_rpc_servlet();

		HttpServletRequest *get_httpServletRequest();

		HttpServletResponse *get_httpServletResponse();

	private:
		virtual bool doGet(HttpServletRequest&, HttpServletResponse&);
		
		virtual bool doPost(HttpServletRequest&, HttpServletResponse&);

		virtual bool doUnknown(HttpServletRequest&, HttpServletResponse&);

		virtual bool doError(HttpServletRequest&, HttpServletResponse&);

		bool read_http_body(HttpServletRequest &req, string &body);

		service_handle *get_handle(const string &message_type, 
			const string &func_name);

		HttpServletRequest *req_;
		HttpServletResponse *resp_;
	};
}