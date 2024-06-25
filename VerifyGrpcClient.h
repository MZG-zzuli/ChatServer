#pragma once

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include <memory>
#include "Singleton.h"
#include"ConfigMgr.h"
#include"const.h"
class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:
    message::GetVerifyRsp GetVerifyCode(std::string email);
    VerifyGrpcClient();
private:
    std::unique_ptr<message::VerifyService::Stub> stub_;
};

