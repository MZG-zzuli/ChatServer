#pragma once

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include <memory>
#include "Singleton.h"
#include"ConfigMgr.h"
#include"const.h"
class RPConPool;
#include"RPConPool.h"
class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:
    message::GetVerifyRsp GetVerifyCode(std::string email);
private:
    VerifyGrpcClient();
    //std::unique_ptr<message::VerifyService::Stub> stub_;
    std::unique_ptr<RPConPool> pool_;
};

