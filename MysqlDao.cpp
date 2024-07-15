#include "MysqlDao.h"


MysqlDao::MysqlDao()
{
    auto& cfg = ConfigMgr::Inst();
    const auto& host = cfg["Mysql"]["Host"];
    const auto& port = cfg["Mysql"]["Port"];
    const auto& pwd = cfg["Mysql"]["Passwd"];
    const auto& schema = cfg["Mysql"]["Schema"];
    const auto& user = cfg["Mysql"]["User"];
    pool_.reset(new MySqlPool(host, user, pwd, schema, 5,stoi(port)));
    
}


MysqlDao::~MysqlDao()
{

    pool_->Close();

}
void clo(MYSQL* con)
{
}
int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
    std::cout << "RegUser" << std::endl;
    std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> conn(pool_->getConnection());
    mysql_autocommit(conn.get(), 0);
    if (!conn)
    {
        return -1;
    }
    if (getUid(email) > 0)
    {
        return -3;
    }
    std::string sql = "INSERT INTO user (name, email, pwd) VALUES ('" + name + "', '" + email + "', '" + pwd + "')";
    if (mysql_query(conn.get(), sql.c_str()))
    {
        return -2;
    }
    mysql_commit(conn.get());
    int uid=getUid(email);
    pool_->returnConnection(conn);
    return uid;
}

int MysqlDao::getUid(const std::string& email)
{
    std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> con(pool_->getConnection());
    std::string sql = "select id from user where email='" + email + "';";
    if (mysql_query(con.get(), sql.c_str()))
    {
        return -1;
        
    }
    MYSQL_RES* res = mysql_store_result(con.get());
    if (!res)
    {
        return -2;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row)
    {
        return -3;
    }
    int uid = atoi(row[0]);
    mysql_free_result(res);
    pool_->returnConnection(con);
    return uid;
}

int MysqlDao::CheckEmail(const std::string& name, const std::string& email)
{
    std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> con(pool_->getConnection());
    std::string sql = "SELECT id FROM user where name='" + name + "' and email='" + email + "';";
    std::cout << sql << std::endl;
    if (mysql_query(con.get(), sql.c_str()))
    {
        std::cout<<1<<std::endl;
        return -1;
    }
    MYSQL_RES* res = mysql_store_result(con.get());
    if (!res)
    {
        std::cout << 2 << std::endl;
        return -1;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row)
    {
        std::cout << 3 << std::endl;
        return -1;
    }
    int uid = atoi(row[0]);
    mysql_free_result(res);
    pool_->returnConnection(con);
    return uid;
}

bool MysqlDao::UpdatePwd(const std::string& email, const std::string& newpwd)
{
    std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> con(pool_->getConnection());
    std::string sql = "UPDATE user SET pwd='" + newpwd + "' where email='" + email + "';";
    if (mysql_query(con.get(), sql.c_str()))
    {
        return false;//Ê§°Ü
    }
    pool_->returnConnection(con);
    return true;
}
