#pragma once
#include"Singleton.h"
#include<boost/asio.hpp>
#include<vector>
#include<iostream>
class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
	friend class Singleton<AsioIOServicePool>;
public:
	boost::asio::io_context& getIOService();
	~AsioIOServicePool();
	void stop();
private:
	AsioIOServicePool(std::size_t size = 2);
	AsioIOServicePool(AsioIOServicePool const&) = delete;
	AsioIOServicePool& operator=(AsioIOServicePool const&) = delete;
	std::vector<boost::asio::io_context> _ioServices;
	std::vector<std::unique_ptr<boost::asio::io_context::work>> _workes;
	std::vector<std::thread> _threads;
	std::size_t _nextServiceIndex;

};

