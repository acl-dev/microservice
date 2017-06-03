#pragma once
namespace acl
{
	class json_service_handles : public service_handles
	{
	public:
		static json_service_handles &get_instance()
		{
			static json_service_handles instance;
			return instance;
		}

		template<class CONTEXT, class REQ, class RESP>
		bool add(const char *service_path, CONTEXT *ctx,
			bool(CONTEXT::*func)(const REQ &, RESP &))
		{
			acl::lock_guard guard(lock_);
			if (exist(service_path))
			{
				logger_error("service_path exist error: %s",
					service_path);
				return false;
			}
			message_handles_.insert(std::make_pair(service_path,
				new json_service_handle<
				CONTEXT, REQ, RESP>(ctx, func)));

			return true;
		}
	private:
		json_service_handles()
		{
		}
		//
		json_service_handles(const json_service_handles &);
		json_service_handles &operator =(const json_service_handles &);

	};
}