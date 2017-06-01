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
		/**
		 * 当 HTTP 请求为 GET 方式时调用的函数
		 */
		virtual bool doGet(HttpServletRequest&, HttpServletResponse&);
		
		/**
		 * 当 HTTP 请求为 POST 方式时调用的函数
		 */
		virtual bool doPost(HttpServletRequest&, HttpServletResponse&);

		/**
		 * 当 HTTP 请求方法未知时调用的虚函数
		 */
		virtual bool doUnknown(HttpServletRequest&, HttpServletResponse&);

		/**
		 * 当 HTTP 请求出错时调用的虚函数
		 */
		virtual bool doError(HttpServletRequest&, HttpServletResponse&);

		/**
		 * 读取 HTTP body
		 */
		bool read_http_body(HttpServletRequest &req, string &body);


		service_handle *get_handle(const string &message_type, 
			const string &func_name);

		HttpServletRequest *req_;
		HttpServletResponse *resp_;
	};
}