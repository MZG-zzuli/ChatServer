#pragma once
#include"Singleton.h"
class HttpConnection;
#include"HttpConnection.h"
#include<map>
#include<Json/json.h>
#include<Json/value.h>
#include<json/reader.h>
#include"const.h"
#include<boost/beast.hpp>


typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	bool HandleGet(std::string url, std::shared_ptr<HttpConnection> response);
	bool HandlePost(std::string url, std::shared_ptr<HttpConnection> response);
	void RegGet(std::string url, HttpHandler handler);
	void RegPost(std::string url, HttpHandler handler);
private:
	LogicSystem();
	std::map<std::string,HttpHandler> _post_handlers;
	std::map<std::string,HttpHandler> _get_handlers;

};

