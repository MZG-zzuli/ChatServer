#pragma once
#include<mysql/mysql.h>
#include<iostream>
class MySqlConn
{

public:
	MySqlConn(const std::string& url, const std::string& user,
		const std::string& pass, const std::string& schema);

	~MySqlConn();
	MYSQL* operator-> ()
	{
		return mysql;
	}
private:
	MYSQL* mysql;
};

