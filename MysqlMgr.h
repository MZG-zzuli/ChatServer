#pragma once
#include"MysqlDao.h"
#include"Singleton.h"
class MysqlMgr:public Singleton<MysqlMgr>
{
    friend class Singleton<MysqlMgr>;
public:
    ~MysqlMgr();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int CheckEmail(const std::string& name, const std::string& email);
    bool UpdatePwd(const std::string& email, const std::string& newpwd);
private:
    MysqlMgr();
    MysqlDao  _dao;
};

