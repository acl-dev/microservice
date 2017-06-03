#pragma once
namespace acl
{
	class protobuf_service_handles : public service_handles
	{
	public:
		static protobuf_service_handles  &get_instance()
		{
			static protobuf_service_handles  instance;
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
				new protobuf_service_handle<
				CONTEXT, REQ, RESP>(ctx, func)));

			return true;
		}
	private:
		protobuf_service_handles()
		{
		}
		//
		protobuf_service_handles(const json_service_handles &);
		protobuf_service_handles  &operator =(const protobuf_service_handles  &);

	};
}