#include "MysqlMgr.h"

MysqlMgr::~MysqlMgr() {

}
int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
    return _dao.RegUser(name, email, pwd);
}
int MysqlMgr::CheckEmail(const std::string& name, const std::string& email)
{
    return _dao.CheckEmail(name,email);
}
bool MysqlMgr::UpdatePwd(const std::string& email, const std::string& newpwd)
{
    return _dao.UpdatePwd(email, newpwd);
}
MysqlMgr::MysqlMgr() {
}