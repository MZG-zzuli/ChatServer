#include "LogicSystem.h"

LogicSystem::LogicSystem()
{
	RegGet("/test",std::bind(&LogicSystem::test,this,std::placeholders::_1));
	RegPost("/get_verifycode", std::bind(&LogicSystem::get_verifycode, this, std::placeholders::_1));
	RegPost("/user_register", std::bind(&LogicSystem::user_register, this, std::placeholders::_1));
	RegPost("/reset_pwd", std::bind(&LogicSystem::reset_pwd, this, std::placeholders::_1));
	RegPost("/user_login", std::bind(&LogicSystem::user_login, this, std::placeholders::_1));

}
bool LogicSystem::test(std::shared_ptr<HttpConnection> connection)
{
	for (std::pair<std::string, std::string> elem : connection->_get_params)
	{
		boost::beast::ostream(connection->_response.body()) << elem.first << " : " << elem.second << std::endl;
		std::cout << elem.first << " : " << elem.second << std::endl;
	}
	return true;
}
bool LogicSystem::get_verifycode(std::shared_ptr<HttpConnection> connection)
{
	std::string json_str = boost::beast::buffers_to_string(connection->_request.body().data());
	std::cout << "--json_str--\n" << json_str << std::endl;
	connection->_response.set(boost::beast::http::field::content_type, "text/json");
	Json::Value json_value;
	Json::Reader reader;

	if (reader.parse(json_str, json_value))
	{
		Json::Value re;
		std::cout << "email\n" << json_value["email"].asString() << std::endl;
		message::GetVerifyRsp reply = VerifyGrpcClient::GetInstance()->GetVerifyCode(json_value["email"].asString());
		std::cout << "error:" << reply.error() << "email:" << reply.email() << "code:" << reply.code() << std::endl;
		if (reply.error() == ErrorCodes::RPCFailed)
		{
			re["error"] = ErrorCodes::RPCFailed;
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

}
bool LogicSystem::user_register(std::shared_ptr<HttpConnection> connection)
{
	std::cout << "user_register" << std::endl;
	std::string json_str = boost::beast::buffers_to_string(connection->_request.body().data());
	connection->_response.set(boost::beast::http::field::content_type, "text/json");
	Json::Value json_value;
	Json::Reader reader;

	if (reader.parse(json_str, json_value))
	{
		Json::Value re;
		std::cout << "email\n" << json_value["email"].asString() << std::endl;
	}
	std::string verify_code = json_value["verifycode"].asString();
	std::string user=json_value["user"].asString();
	std::string passwd=json_value["passwd"].asString();
	std::string email=json_value["email"].asString();
	std::cout << "verify_code:" << verify_code << "user:" << user << "passwd:" << passwd << "email:" << email << std::endl;

	std::string veriy_redis;
	bool get_redis_verify = RedisMgr::GetInstance()->Get("code_"+email, veriy_redis);
	std::cout<<"key:"<<"code_"<<email << "verify_redis:" << veriy_redis <<"re:"<<get_redis_verify<< std::endl;
	Json::Value re;
	if(!get_redis_verify)
	{
		re["error"]=ErrorCodes::VerifyExpired;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		return true;
	}
	if (verify_code != veriy_redis)
	{
		re["error"] = ErrorCodes::VerifyCodeErr;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		return true;
	}
	int uid = MysqlMgr::GetInstance()->RegUser(user, email, passwd);
	std::cout << "uid" << uid<<std::endl;
	if (uid <= 0)
	{
		re["error"] = ErrorCodes::UserExist;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		return true;
	}
	re["error"] = ErrorCodes::Success;
	re["uid"] = uid;
	re["user"] = user;
	re["email"] = email;
	re["passwd"]=passwd;
	re["verifycode"]=veriy_redis;
	boost::beast::ostream(connection->_response.body()) << re.toStyledString();
	return true;
}

bool LogicSystem::reset_pwd(std::shared_ptr<HttpConnection> connection)
{
	std::cout << "reset_code http" << std::endl;
	std::string json_str=boost::beast::buffers_to_string(connection->_request.body().data());
	connection->_response.set(boost::beast::http::field::content_type, "text/json");
	Json::Value value;
	Json::Reader reader;
	reader.parse(json_str, value);
	std::string verify_code;
	Json::Value re;
	bool get_redis_verify = RedisMgr::GetInstance()->Get("code_" + value["email"].asString(), verify_code);
	if (!get_redis_verify)
	{
		re["error"]=ErrorCodes::VerifyExpired;
		boost::beast::ostream(connection->_response.body())<<re.toStyledString();
		return true;
	}
	if (value["verifycode"].asString() != verify_code)
	{
		re["error"]=ErrorCodes::VerifyCodeErr;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		return true;
	}
	int check_user_id = MysqlMgr::GetInstance()->CheckEmail(value["name"].asString(), value["email"].asString());
	if (check_user_id < 0)
	{
		re["error"] = ErrorCodes::UserError;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		return true;
	}
	bool update_pwd=MysqlMgr::GetInstance()->UpdatePwd(value["email"].asString(), value["newpwd"].asString());
	if (!update_pwd)
	{
		re["error"] = ErrorCodes::UserError;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		return true;

	}
	re["error"] = ErrorCodes::Success;
	boost::beast::ostream(connection->_response.body()) << re.toStyledString();
	return true;
}
	
bool LogicSystem::user_login(std::shared_ptr<HttpConnection> connection)
{
	std::cout << "user_login\n";
	std::string json_str = boost::beast::buffers_to_string(connection->_request.body().data());
	Json::Reader reader;
	Json::Value value;
	Json::Value re;
	bool parse_success = reader.parse(json_str, value);
	if (!parse_success)
	{
		re["error"] = ErrorCodes::Error_Json;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		std::cout << "json error\n";
		return true;
	}
	std::string email = value["email"].asString();
	std::string passwd = value["passwd"].asString();
	std::cout << email << ":" << passwd;
	//------数据库验证-----
	UserInfo user;
	bool pwd_valid = MysqlMgr::GetInstance()->CheckPwd(email, passwd, user);
	if (!pwd_valid)
	{
		re["error"] = ErrorCodes::PasswdErr;
		boost::beast::ostream(connection->_response.body()) << re.toStyledString();
		std::cout << "password error\n";
		return true;
	}

	//--------grpc获取合适服务器并返回
	message::GetChatServerRsp reply = StatusGrpcClient::GetInstance()->GetChatServer(user.uid);
	if (reply.error())
	{
		std::cout << "getChatServer error!" << std::endl;
		re["error"] = ErrorCodes::RPCGetFailed;
		std::string jsonstr = re.toStyledString();
		boost::beast::ostream(connection->_response.body()) << jsonstr;
		return true;
	}
	re["error"] = ErrorCodes::Success;
	re["name"] = user.name;
	re["uid"] = user.uid;
	re["token"] = reply.token();
	re["host"] = reply.host();
	re["port"] = reply.port();
	std::cout << re.toStyledString() << std::endl;
	boost::beast::ostream(connection->_response.body()) << re.toStyledString();

	return true;
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
