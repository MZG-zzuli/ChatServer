#pragma once
#include"const.h"
#include"Singleton.h"
#include"ConfigMgr.h"
#include"message.grpc.pb.h"
#include"message.pb.h"
#include"StatusConPool.h"

class StatusGrpcClient:public Singleton<StatusGrpcClient>
{
	friend class Singleton<StatusGrpcClient>;
public:
	~StatusGrpcClient();
	message::GetChatServerRsp GetChatServer(int uid);
private:
	StatusGrpcClient();
	std::unique_ptr<StatusConPool> pool_;
};

