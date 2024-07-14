#include "MySqlConn.h"
MySqlConn::MySqlConn(const std::string& url, const std::string& user,
	const std::string& pass, const std::string& schema)
{
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, url.c_str(), user.c_str(), pass.c_str(), schema.c_str(), 0, NULL, 0);

}

MySqlConn::~MySqlConn()
{
	mysql_close(mysql);
}


