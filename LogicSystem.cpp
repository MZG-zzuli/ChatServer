#include "LogicSystem.h"

LogicSystem::LogicSystem()
{

	RegGet("/test", [](std::shared_ptr<HttpConnection> connection) {
		boost::beast::ostream(connection->_response.body()) << "hello world";
		});
}
LogicSystem::~LogicSystem()
{
}
bool LogicSystem::HandleGet(std::string url, std::shared_ptr<HttpConnection> response)
{
	if (_get_handlers.find(url) == _get_handlers.end())
	{
		return false;
	}
	_get_handlers[url](response);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
	_get_handlers[url] = handler;
}
