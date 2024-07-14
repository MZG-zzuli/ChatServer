
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
void TestRedisMgr() {

    assert(RedisMgr::GetInstance()->Auth("123456"));
    assert(RedisMgr::GetInstance()->Set("blogwebsite", "llfc.club"));
    std::string value = "";
    assert(RedisMgr::GetInstance()->Get("blogwebsite", value));
    assert(RedisMgr::GetInstance()->Get("nonekey", value) == false);
    assert(RedisMgr::GetInstance()->HSet("bloginfo", "blogwebsite", "llfc.club"));
    assert(RedisMgr::GetInstance()->HGet("bloginfo", "blogwebsite") != "");
    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo"));
    assert(RedisMgr::GetInstance()->Del("bloginfo"));
    assert(RedisMgr::GetInstance()->Del("bloginfo"));
    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo") == false);
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->LPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->LPop("lpushkey2", value) == false);
    RedisMgr::GetInstance()->Close();
}
int main()
{
    std::string s;
    RedisMgr::GetInstance()->Get("code_208474482@qq.com", s);
    std::cout << s << std::endl;
    std::cout << "--------------\n";
    //TestRedisMgr();
    
    
	std::cout << ConfigMgr::Inst()["GateServer"]["port"];
	boost::asio::io_context io_context;

	std::make_shared<CServer>(io_context, stoi(ConfigMgr::Inst()["GateServer"]["port"]))->start();
	std::cout << "GateServer start" << std::endl;
	io_context.run();
	return 0;
}



