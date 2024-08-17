#include "StatusGrpcClient.h"

StatusGrpcClient::~StatusGrpcClient()
{
}

message::GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
    grpc::ClientContext context;
    message::GetChatServerReq request;
    message::GetChatServerRsp reply;
    std::unique_ptr<message::StatusService::Stub> stub = pool_->getConnection();
    stub->GetChatServer(&context, request, &reply);


    return reply;
}
StatusGrpcClient::StatusGrpcClient()
{
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string host = gCfgMgr["StatusServer"]["Host"];
    std::string port = gCfgMgr["StatusServer"]["Port"];
    pool_.reset(new StatusConPool(5, host, port));
}