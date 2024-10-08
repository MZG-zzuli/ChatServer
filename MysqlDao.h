#pragma once
#include<iostream>
#include"MySqlPool.h"
#include"ConfigMgr.h"

struct UserInfo {
    std::string name;
    std::string pwd;
    int uid;
    std::string email;
};
class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int getUid(const std::string& email);
    int CheckEmail(const std::string& name, const std::string& email);
    bool UpdatePwd(const std::string& email, const std::string& newpwd);
    bool CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo);
private:
    std::unique_ptr<MySqlPool> pool_;
};

