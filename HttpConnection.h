#pragma once

#include<memory>
#include<boost/asio.hpp>
#include<boost/beast.hpp>
#include<iostream>
#include<unordered_map>
class LogicSystem;
#include"LogicSystem.h"
class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(boost::asio::io_context& ioc);
	~HttpConnection();
	boost::asio::ip::tcp::socket& getSocket();
	void start();

private:
	boost::beast::flat_buffer _buffer;
	boost::beast::http::request<boost::beast::http::dynamic_body> _request;
	boost::beast::http::response<boost::beast::http::dynamic_body> _response;
	std::string _get_url;
	std::unordered_map<std::string,std::string> _get_params;
	boost::asio::ip::tcp::socket _sock;
	unsigned char ToHex(unsigned char x);
	unsigned char FromHex(unsigned char x);
	std::string UrlEncode(const std::string& str);
	std::string UrlDecode(const std::string& str);
	void CheckDeadline();
	void WriteResponse();
	void HandleReq();
	void PreParseGetParam();
	boost::asio::steady_timer deadline_{ _sock.get_executor(),std::chrono::seconds(60) };
};

