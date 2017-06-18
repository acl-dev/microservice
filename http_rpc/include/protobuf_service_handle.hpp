#pragma once
namespace acl
{
	template<class CONTEXT, class REQ, class RESP>
	class protobuf_service_handle: public service_handle
	{
	public:
		typedef bool (CONTEXT::*func_t)(const REQ &, RESP&);

		protobuf_service_handle(CONTEXT *ctx, func_t fun)
			:CONTEXT_(ctx),
			function_(fun)
		{
		}

		virtual bool invoke(const string &data, string &buffer)
		{
			REQ req;
			RESP resp;

			if (!req.ParseFromArray(data.c_str(), static_cast<int>(data.size())))
			{
				logger_error("ParseFromArray failed");
				return false;
			}
			//invoke 
			bool ret = (CONTEXT_->*function_)(req, resp);

			std::string str = resp.SerializeAsString();
			buffer.append(str.data(), str.size());

			return ret;
		}

	private:
		CONTEXT *CONTEXT_;
		func_t function_;
	};
}