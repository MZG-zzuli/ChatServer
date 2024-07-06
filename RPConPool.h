#pragma once
#include"VerifyGrpcClient.h"
#include"message.grpc.pb.h"
#include<memory>
#include<queue>
#include<condition_variable>
#include<mutex>
#include<atomic>
class RPConPool
{
public:
    RPConPool(size_t pool_size, const std::string& host, const std::string& port);
    ~RPConPool();
    std::unique_ptr < message::VerifyService::Stub> getConnection();
    void returnConnection(std::unique_ptr < message::VerifyService::Stub>& context);
    void close();
private:
    std::atomic<bool> b_stop_;
    std::size_t pool_size;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr< message::VerifyService::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

