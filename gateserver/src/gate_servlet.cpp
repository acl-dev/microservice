#include "http_rpc.h"
#include "config.h"
#include "gate_servlet.h"

gate_servlet::gate_servlet()
	:auth_key_(var_cfg_auth_key)
{

}

bool gate_servlet::doUnknown(acl::HttpServletRequest&,
	acl::HttpServletResponse& res)
{
	return false;
}
bool gate_servlet::doGet(acl::HttpServletRequest& req,
	acl::HttpServletResponse& res)
{
	const char *error_str = "405 Method Not Allowed";

	res.setStatus(415)
		.setContentType("text/plain").
		setContentLength(strlen(error_str)).
		write(error_str, strlen(error_str));
	return false;
}

static bool read_http_body(acl::HttpServletRequest& req, acl::string &body)
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
bool gate_servlet::doPost(acl::HttpServletRequest& req,
	acl::HttpServletResponse& res)
{

	acl::string body;
	acl::string buffer;

	//用户身份认证
	if (!check_auth(req, res))
	{
		logger("check_auth failed");
		return false;
	}
		
	
	if (read_http_body(req, body) == false)
	{
		const char * error_str = "400 Bad Request";
		res.setStatus(400)
			.setContentType("text/plain").
			setContentLength(strlen(error_str)).
			write(error_str, strlen(error_str));
		return false;
	}
		

	acl::string content_type = req.getContentType(false);
	if (strcmp(content_type.c_str(), "application/json") != 0)
	{
		const char * error_str = "415 Unsupported Content-Type";
		res.setStatus(415)
			.setContentType("text/plain").
			setContentLength(strlen(error_str)).
			write(error_str, strlen(error_str));
		return false;
	}

	acl::http_rpc_client::status_t status = 
		acl::http_rpc_client::get_instance().
		json_call(req.getPathInfo(),body,buffer);

	if (!status)
	{
		logger("json_call falied: service:%s ",req.getPathInfo());
		acl::string error_str = "404 Not Found";
		res.setStatus(404).
			setContentLength(status.error_str_.size()).
			setContentType("text/plain").
			write(status.error_str_);
		return false;
	}

	res.setStatus(200)
		.setKeepAlive(req.isKeepAlive())
		.setContentLength(buffer.length());
	if (!res.write(buffer) || !req.isKeepAlive())
		return false;
	return true;
}
bool gate_servlet::doError(acl::HttpServletRequest& req,
	acl::HttpServletResponse&res)
{
	return false;
}

bool gate_servlet::check_auth(acl::HttpServletRequest& req,
	acl::HttpServletResponse& res)
{
	if (auth_key_.size())
	{
		acl::string value = req.getSession().getAttribute(auth_key_);
		if (value.empty())
		{
			const char *error_str = "401 Unauthorized";
			res.setStatus(401).
				setContentType("text/plain").
				setContentLength(strlen(error_str)).
				write(error_str);
			return false;
		}
	}
	return true;
}