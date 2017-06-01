#pragma once
class gate_servlet : public acl::HttpServlet
{
public:
	gate_servlet();
protected:
	virtual bool doUnknown(acl::HttpServletRequest&,
		acl::HttpServletResponse& res);
	virtual bool doGet(acl::HttpServletRequest& req,
		acl::HttpServletResponse& res);
	virtual bool doPost(acl::HttpServletRequest& req,
		acl::HttpServletResponse& res);
	virtual bool doError(acl::HttpServletRequest& req,
		acl::HttpServletResponse&res);

private:
	bool check_auth(acl::HttpServletRequest& req,
		acl::HttpServletResponse& res);

	acl::string auth_key_;
};