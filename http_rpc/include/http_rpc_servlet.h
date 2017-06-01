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
		 * �� HTTP ����Ϊ GET ��ʽʱ���õĺ���
		 */
		virtual bool doGet(HttpServletRequest&, HttpServletResponse&);
		
		/**
		 * �� HTTP ����Ϊ POST ��ʽʱ���õĺ���
		 */
		virtual bool doPost(HttpServletRequest&, HttpServletResponse&);

		/**
		 * �� HTTP ���󷽷�δ֪ʱ���õ��麯��
		 */
		virtual bool doUnknown(HttpServletRequest&, HttpServletResponse&);

		/**
		 * �� HTTP �������ʱ���õ��麯��
		 */
		virtual bool doError(HttpServletRequest&, HttpServletResponse&);

		/**
		 * ��ȡ HTTP body
		 */
		bool read_http_body(HttpServletRequest &req, string &body);


		service_handle *get_handle(const string &message_type, 
			const string &func_name);

		HttpServletRequest *req_;
		HttpServletResponse *resp_;
	};
}