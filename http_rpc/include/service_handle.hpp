#pragma once
namespace acl
{
	/*
	* 消息处理器的基类。子类集成，并实现invoke 函数。
	* json_msg_handler 继承了，并实现了invoke函数，就可以处理json消息
	*/
	struct service_handle
	{
		/*
		* 处理消息的接口
		* @param {const string@} req 网络发送过来的请求。
		* @param {string@} resp  将要发送回去的相应。
		# @return {bool} false 将会断开连接，true保持链接
		*/
		virtual bool invoke(const string &req, string &resp) = 0;
	};
}