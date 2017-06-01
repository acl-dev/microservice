#include "http_rpc.h"
access_list::access_list()
: allow_all_(false)
{
	allow_clients_ = acl_iplink_create(10);
}

access_list::~access_list()
{
	acl_iplink_free(allow_clients_);
}

void access_list::add_user(const char* user)
{
	acl::string buf(user);
	buf.lower();

	std::vector<acl::string>::const_iterator cit = white_list_.begin();
	for (; cit != white_list_.end(); ++cit)
	{
		if ((*cit) == buf)
			return;
	}

	white_list_.push_back(buf);
}

void access_list::set_allow_clients(const char* iplist)
{
	if (strcasecmp(iplist, "all") == 0)
	{
		const char* from = "0.0.0.0", *to = "255.255.255.255";

		if (acl_iplink_insert(allow_clients_, from, to) == NULL)
			logger_warn("invalid ip item: %s:%s", from, to);
		else
			logger("add allow client: %s, %s", from, to);

		return;
	}

	acl::string buf(iplist);
	std::vector<acl::string>& tokens = buf.split2(",; \t");
	std::vector<acl::string>::iterator it = tokens.begin();
	acl::string from, to;
	for (; it != tokens.end(); ++it)
	{
		int at = (*it).find(':');
		if (at <= 0)
		{
			logger_warn("invalid token: %s", (*it).c_str());
			continue;
		}
		if ((*it).substr(from, 0, at) == 0)
			continue;
		if ((*it).substr(to, at + 1, (*it).length() - at) == 0)
			continue;
		if (acl_iplink_insert(allow_clients_, from.c_str(),
			to.c_str()) == NULL)
		{
			logger_warn("invalid from: %s, to: %s",
				from.c_str(), to.c_str());
		}
		else
			logger("add allow client: %s, %s",
				from.c_str(), to.c_str());
		from.clear();
		to.clear();
	}
}
bool access_list::check_client(const char* ip)
{
	return acl_iplink_lookup_str(allow_clients_, ip) == NULL
		? false : true;
}