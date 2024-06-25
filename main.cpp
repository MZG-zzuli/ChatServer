
#include<boost/asio.hpp>
#include"CServer.h"
#include<memory>
#include<boost/asio.hpp>
#include"VerifyGrpcClient.h"
#include"const.h"
#include"ConfigMgr.h"
#include<iostream>


ConfigMgr config;
int main()
{
	std::cout << config["GateServer"]["port"];
	std::shared_ptr<VerifyGrpcClient> verifyGrpcClient=std::make_shared<VerifyGrpcClient>();
	verifyGrpcClient->GetVerifyCode("dd");
	boost::asio::io_context io_context;


	std::make_shared<CServer>(io_context, stoi(config["GateServer"]["port"]))->start();
	io_context.run();
	return 0;
}



