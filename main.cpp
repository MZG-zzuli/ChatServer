
#include<boost/asio.hpp>
#include"CServer.h"
#include<memory>
#include<boost/asio.hpp>


#include<iostream>

void Print(std::error_code ec) {
	std::cout << "Hello, world!" << std::endl;
}
int main()
{
	boost::asio::io_context io_context;


	std::make_shared<CServer>(io_context, 80)->start();
	io_context.run();
	while (1);
	return 0;
}