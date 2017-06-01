#pragma once

namespace acl
{
	class service_servlet : public acl::HttpServlet
	{
	public:
		service_servlet();
	protected:
		virtual bool doGet(HttpServletRequest&, HttpServletResponse&);

		virtual bool doPost(HttpServletRequest&, HttpServletResponse&);
	private:

		bool publish(HttpServletRequest&, HttpServletResponse&);

		bool update_config(HttpServletRequest&, HttpServletResponse&);

		bool stop(HttpServletRequest&, HttpServletResponse&);

		bool del(HttpServletRequest&, HttpServletResponse&);


	};
}