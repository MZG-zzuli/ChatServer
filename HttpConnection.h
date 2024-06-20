#pragma once

#include<memory>
#include<boost/asio.hpp>
#include<boost/beast.hpp>
#include<iostream>
class LogicSystem;
#include"LogicSystem.h"
class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(boost::asio::ip::tcp::socket& sock);
	void start();

private:
	boost::beast::flat_buffer _buffer;
	boost::beast::http::request<boost::beast::http::dynamic_body> _request;
	boost::beast::http::response<boost::beast::http::dynamic_body> _response;
	boost::asio::ip::tcp::socket _sock;
	void CheckDeadline();
	void WriteResponse();
	void HandleReq();
	boost::asio::steady_timer deadline_{ _sock.get_executor(),std::chrono::seconds(60) };
};

