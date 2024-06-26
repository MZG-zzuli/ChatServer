#include "LogicSystem.h"

LogicSystem::LogicSystem()
{

	RegGet("/test", [](std::shared_ptr<HttpConnection> connection) {
		for (std::pair<std::string, std::string> elem:connection->_get_params)
		{
			boost::beast::ostream(connection->_response.body())<<elem.first<<" : "<<elem.second<<std::endl;
			std::cout<<elem.first<<" : "<<elem.second<<std::endl;
		}
		});
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
		std::string json_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout<<"--json_str--\n" << json_str << std::endl;
		connection->_response.set(boost::beast::http::field::content_type, "text/json");
		Json::Value json_value;
		Json::Reader reader;
		
		if (reader.parse(json_str, json_value))
		{
			Json::Value re;
			std::cout<<"email\n"<<json_value["email"].asString()<<std::endl;
			message::GetVerifyRsp reply = VerifyGrpcClient::GetInstance()->GetVerifyCode(json_value["email"].asString());
			std::cout<<"error:"<<reply.error()<<"email:"<<reply.email()<<"code:"<<reply.code()<<std::endl;
			if (reply.error() == ErrorCodes::RPCFailed)
			{
				re["error"]=ErrorCodes::RPCFailed;
			}
			else
			{
				re["error"] = ErrorCodes::Success;
			}
			boost::beast::ostream(connection->_response.body()) << re.toStyledString();

			return true;
		}
		else
		{
			Json::Value re;
			re["error"] = ErrorCodes::Error_Json;
			boost::beast::ostream(connection->_response.body()) << re.toStyledString();
			return false;

		}
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

bool LogicSystem::HandlePost(std::string url, std::shared_ptr<HttpConnection> response)
{
	if (_post_handlers.find(url) == _post_handlers.end())
	{
		return false;
	}
	_post_handlers[url](response);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
	_get_handlers[url] = handler;
}

void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
	_post_handlers[url] = handler;
}
