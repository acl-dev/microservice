#include "http_rpc.h"

namespace acl
{
	extern acl_pthread_key_t req_key;

	service_base::~service_base()
	{

	}

	HttpServletRequest & service_base::getHttpServletRequest()
	{
		void *ptr = acl_pthread_getspecific(req_key);
		acl_assert(ptr);

		return *(HttpServletRequest*)ptr;
	}
}