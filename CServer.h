#pragma once
#include<boost/beast.hpp>
#include<boost/asio.hpp>
#include<memory>
#include<iostream>
#include"AsioIOServicePool.h"
class HttpConnection;
#include"HttpConnection.h"
class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, int port);
	void start();

private:
	boost::asio::ip::tcp::acceptor _accept;

	boost::asio::io_context& _ioc;

};

