#pragma once
#include<iostream>
#include<memory>
#include<mysql/mysql.h>
#include<queue>
#include<mutex>
#include<functional>
#include"MysqlConn.h"
class MySqlPool
{
public:
    MySqlPool(const std::string& url, const std::string& user, const std::string& pass,
        const std::string& schema, int poolSize,const int& port=3306);
    ~MySqlPool();
    std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> getConnection();
    void returnConnection(std::unique_ptr<MYSQL,std::function<void(MYSQL*)>>& con);
    void Close();
private:
    std::string url_;
    std::string user_;
    std::string pass_;
    std::string schema_;
    int poolSize_;
    std::queue<std::unique_ptr<MYSQL, std::function<void(MYSQL*)>>> pool_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> b_stop_;
    std::function<void(MYSQL*)> closeMysql= [](MYSQL* con) {
	mysql_close(con);
	};
};

