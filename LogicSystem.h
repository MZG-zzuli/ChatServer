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
#include"message.pb.h"
#include"VerifyGrpcClient.h"
#include"MysqlMgr.h"
#include"RedisMgr.h"
#include"StatusGrpcClient.h"
typedef std::function<bool(std::shared_ptr<HttpConnection>)> HttpHandler;
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

	bool test(std::shared_ptr<HttpConnection> connection);
	bool get_verifycode(std::shared_ptr<HttpConnection> connection);
	bool user_register(std::shared_ptr<HttpConnection> connection);
	bool reset_pwd(std::shared_ptr<HttpConnection> connection);
	bool user_login(std::shared_ptr<HttpConnection> connectioon);
	std::map<std::string,HttpHandler> _post_handlers;
	std::map<std::string,HttpHandler> _get_handlers;

};

