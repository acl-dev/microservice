#include "http_rpc.h"

namespace acl
{

	http_rpc_servlet::http_rpc_servlet(socket_stream* stream, session* session)
		: HttpServlet(stream, session),
          req_(NULL),
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
			return json_service_handles::get_instance().
				get_handle(func_name);
				
		}else if(message_type == "application/x-protobuf")
		{
			return protobuf_service_handles::get_instance().
				get_handle(func_name);
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
        const char *content_type = req.getContentType(false);
        const char *path = req.getPathInfo();
        long long int content_length = req.getContentLength();

		service_handle *handle = get_handle(content_type, path);

        if (handle == NULL)
        {
            logger_error("can't find message. path: %s", path);

            acl::string buf;

            buf += path;
            buf += (". 404 Not Found");

            resp.setStatus(404);
            resp.setContentType("text/html; charset=utf-8");
            resp.setContentLength((long long int) buf.size());
            resp.setKeepAlive(false);

            if (!resp.sendHeader())
                return false;

            (void)resp.getOutputStream().write(buf);
            return false;
		}

        acl::istream& in = req.getInputStream();
        string body;
        char  buf[8192];
        long long int length = content_length;

        while (length > 0)
        {
            size_t n = (size_t)length > sizeof(buf) - 1
                    ? sizeof(buf) - 1 : (size_t)length;
            int ret = in.read(buf, n, false);
            if (ret == -1)
            {
                logger_error("read_http_body error."
                             "service_path:%s "
                             "content_type:%s "
                             "content_length:%llu",
                             path,
                             content_type,
                             content_length);

                return false;
            }
            body.append(buf, ret);
            length -= ret;
        }

		//create acl_pthread_key_create
		acl_pthread_once(&once_control, once_init);
		acl_pthread_setspecific(req_key, &req);

        string buffer;
        bool ret = handle->invoke(body, buffer);

		resp.setStatus(200)
			.setKeepAlive(req.isKeepAlive())
			.setContentLength((long long int) buffer.length());
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

}
