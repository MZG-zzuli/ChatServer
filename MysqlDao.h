#pragma once
#include<iostream>
#include"MySqlPool.h"
#include"ConfigMgr.h"
class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int getUid(const std::string& email,MYSQL* con);

private:
    std::unique_ptr<MySqlPool> pool_;
};

