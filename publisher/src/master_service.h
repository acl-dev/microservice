#pragma once

namespace acl
{
	class master_service: acl::master_threads
	{
	public:
		master_service();
	protected:
		virtual bool thread_on_read(socket_stream* stream);

		virtual bool thread_on_accept(socket_stream* stream);

		virtual bool thread_on_timeout(socket_stream* stream);

		virtual void thread_on_close(socket_stream* stream);

		virtual void thread_on_init();

		virtual void thread_on_exit();

		virtual bool proc_exit_timer(size_t nclients, size_t nthreads);

		virtual void proc_on_init() override;

		virtual void proc_on_exit() override;

	private:

	};
}