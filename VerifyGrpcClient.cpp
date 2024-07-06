#include "VerifyGrpcClient.h"

message::GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email)
{
	message::GetVerifyRsp reply;
	grpc::ClientContext context;
	message::GetVerifyReq request;
	request.set_email(email);
	std::unique_ptr<message::VerifyService::Stub> stub_=pool_->getConnection();
	grpc::Status status=stub_->GetVerifyCode(&context,request,&reply);
	if(status.ok())
	{
		std::cout<<"GetVerifyCode Success"<<std::endl;
		pool_->returnConnection(stub_);
		return reply;
	}
	else
	{
		std::cout<<"GetVerifyCode Failed"<<std::endl;
		reply.set_error(ErrorCodes::RPCFailed);
		pool_->returnConnection(stub_);

		return reply;
	}
	return reply;
}
VerifyGrpcClient::VerifyGrpcClient()
{
	ConfigMgr& con = ConfigMgr::Inst();
	pool_.reset(new RPConPool(5, con["VerifyServer"]["ip"], con["VerifyServer"]["port"]));
}
