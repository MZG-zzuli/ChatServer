#include "MySqlPool.h"

MySqlPool::MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize, const int& port)
	:url_(url), user_(user), pass_(pass), schema_(schema), poolSize_(poolSize), b_stop_(false)
{


	for (int i = 0; i < poolSize_; i++)
	{
		MYSQL* con = mysql_init(nullptr);
		if (con == NULL)
		{
			printf("mysql_init() error\n");
		}
		std::cout << "----------" << con << std::endl;
		con = mysql_real_connect(con, url_.c_str(), user_.c_str(), pass_.c_str(), schema_.c_str(), port, NULL, 0);

		pool_.push(std::unique_ptr<MYSQL, std::function<void(MYSQL*)>>(con,closeMysql));
		std::cout << "Connect to MySQL server successfully."<<con << std::endl;
	}
}

MySqlPool::~MySqlPool()
{
	std::unique_lock<std::mutex> lock(mutex_);
	while (!pool_.empty())
	{
		//mysql_close(pool_.front().get());
		pool_.pop();
	}
}

std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> MySqlPool::getConnection()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this]() {
		return !pool_.empty() || b_stop_; });
	if (b_stop_)
	{
		
		return nullptr;
	}
	std::unique_ptr<MYSQL, std::function<void(MYSQL*)>> con = std::move(pool_.front());
	pool_.pop();
	return con;

}

void MySqlPool::returnConnection(std::unique_ptr<MYSQL, std::function<void(MYSQL*)>>& con)
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (b_stop_)
	{
		mysql_close(con.get());
		return;
	}
	pool_.push(std::move(con));
	cond_.notify_one();
}

void MySqlPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();


}
