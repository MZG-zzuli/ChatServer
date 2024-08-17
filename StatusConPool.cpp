#include "StatusConPool.h"



StatusConPool::StatusConPool(size_t pool_size, const std::string& host, const std::string& port) :
	pool_size(pool_size),
	host_(host),
	port_(port),
	b_stop_(false)
{
	std::string ip_port = host + ":" + port;
	for (size_t i = 0; i < pool_size; ++i)
	{
		std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(ip_port, grpc::InsecureChannelCredentials());
		connections_.push(message::StatusService::NewStub(channel));
	}
}

StatusConPool::~StatusConPool()
{
	std::lock_guard<std::mutex> lock(mutex_);
	close();
	while (!connections_.empty())
	{
		connections_.pop();
	}
}

std::unique_ptr<message::StatusService::Stub> StatusConPool::getConnection()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this]()->bool {
		if (b_stop_) {
			return true;
		}
		return !connections_.empty();
		});
	if (b_stop_)return nullptr;
	std::unique_ptr<message::StatusService::Stub> context = std::move(connections_.front());
	connections_.pop();
	return context;


}

void StatusConPool::returnConnection(std::unique_ptr<message::StatusService::Stub>& context)
{
	std::lock_guard<std::mutex> lock(mutex_);
	connections_.push(std::move(context));
	cond_.notify_one();
}

void StatusConPool::close()
{
	b_stop_ = true;
	cond_.notify_all();
}