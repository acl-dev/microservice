#pragma once
namespace acl
{
	template<class CONTEXT, class REQ, class RESP>
	class json_service_handle: public service_handle
	{
	public:
		typedef bool (CONTEXT::*func_t)(const REQ &, RESP&);

		json_service_handle(CONTEXT *ctx, func_t fun)
			:context_(ctx),
			function_(fun)
		{
		}

		virtual bool invoke(const string &data, string &buffer)
		{
			REQ req;
			RESP resp;

			//data --> req obj
			std::pair<bool, std::string> status = gson(data, req);
			if (!status.first)
			{
				logger_error("gson error: %s", status.second.c_str());
				return false;
			}

			//invoke 
			bool ret = (context_->*function_)(req, resp);

			//resp obj --> string
			buffer = gson(resp);
			return ret;
		}

	private:
		CONTEXT *context_;
		func_t function_;
	};
}