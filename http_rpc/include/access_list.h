#pragma once


class access_list : public acl::singleton<access_list>
{
public:
	access_list();
	~access_list();

	void set_allow_clients(const char* iplist);
	bool check_client(const char* ip);

private:
	bool allow_all_;
	std::vector<acl::string> white_list_;
	ACL_IPLINK* allow_clients_;

	void add_user(const char* user);
};
