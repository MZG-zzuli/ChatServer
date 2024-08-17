
#include<boost/asio.hpp>
#include"CServer.h"
#include<memory>
#include<boost/asio.hpp>
#include"VerifyGrpcClient.h"
#include"const.h"
#include"ConfigMgr.h"
#include<iostream>
#include<hiredis/hiredis.h>
#include"RedisMgr.h"
#include<assert.h>
#include"MysqlDao.h"
#include"MysqlMgr.h"

int main()
{

	std::cout << ConfigMgr::Inst()["GateServer"]["port"];
	boost::asio::io_context io_context;

	std::make_shared<CServer>(io_context, stoi(ConfigMgr::Inst()["GateServer"]["port"]))->start();
	std::cout << "GateServer start" << std::endl;
	io_context.run();
	return 0;
}



