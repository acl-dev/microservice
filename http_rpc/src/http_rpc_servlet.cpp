#include "http_rpc.h"

namespace acl
{

	http_rpc_servlet::http_rpc_servlet()
		:req_(NULL),
		resp_(NULL)
	{
	}

	HttpServletRequest * http_rpc_servlet::get_httpServletRequest()
	{
		return req_;
	}

	HttpServletResponse * http_rpc_servlet::get_httpServletResponse()
	{
		return resp_;
	}

	service_handle * http_rpc_servlet::get_handle(
		const string &message_type, const string &func_name)
	{
		if (message_type == "application/json")
		{
			return json_service_handles::get_instance()
				.get_handle(func_name);
		}
		logger_error("not message type :%s", message_type.c_str());
		return NULL;
	}

	bool http_rpc_servlet::doGet(HttpServletRequest& req, 
		HttpServletResponse& resp)
	{
		return doPost(req, resp);
	}

	acl_pthread_key_t req_key;
	static void once_init(void)
	{
		acl_pthread_key_create(&req_key, NULL);
	}

	static acl_pthread_once_t once_control = ACL_PTHREAD_ONCE_INIT;
	
	bool http_rpc_servlet::doPost(HttpServletRequest& req, 
		HttpServletResponse& resp)
	{
		service_handle *handle = get_handle(
			req.getContentType(false), 
			req.getPathInfo());

		if (!handle)
		{
			logger_error("can't find message "
				"handle for path: %s", req.getPathInfo());

			acl::string buf("404 Not Found");

			buf += req.getPathInfo();

			resp.setStatus(404);
			resp.setContentType("text/html; charset=utf-8");
			resp.setContentLength(buf.size());
			resp.setKeepAlive(false);
			// 发送 http 响应头
			if (resp.sendHeader() == false)
				return false;
			// 发送 http 响应体
			(void)resp.getOutputStream().write(buf);
			return false;
		}

		string body;
		string buffer;

		if(read_http_body(req, body) == false)
			return false;
		
		//create acl_pthread_key_create
		acl_pthread_once(&once_control, once_init);
		acl_pthread_setspecific(req_key, &req);

		bool ret = handle->invoke(body, buffer);

		resp.setStatus(200)
			.setKeepAlive(req.isKeepAlive())
			.setContentLength(buffer.length());
		if(!resp.write(buffer) || !req.isKeepAlive())
			return false;
		return ret;
	}

	bool http_rpc_servlet::doUnknown(HttpServletRequest&,
		HttpServletResponse&)
	{
		logger_error("http Unknown error");
		return false;
	}

	bool http_rpc_servlet::doError(HttpServletRequest&,
		HttpServletResponse&)
	{
		return false;
	}

	bool http_rpc_servlet::read_http_body(
		HttpServletRequest &req, string &body)
	{
		long long int length = req.getContentLength();
		if (length <= 0)
		{
			logger_error("length: %lld invalid", length);
			return false;
		}
		acl::istream& in = req.getInputStream();
		char  buf[8192];
		int   ret;
		size_t n;

		while (length > 0)
		{
			n = (size_t)length > sizeof(buf) - 1
				? sizeof(buf) - 1 : (size_t)length;
			ret = in.read(buf, n, false);
			if (ret == -1)
			{
				logger_error("read json body error");
				return false;
			}
			body.append(buf, ret);
			length -= ret;
		}
		return true;
	}

}
