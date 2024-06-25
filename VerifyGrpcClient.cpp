#include "VerifyGrpcClient.h"

message::GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email)
{
	message::GetVerifyRsp reply;
	grpc::ClientContext context;
	message::GetVerifyReq request;
	request.set_email(email);
	grpc::Status status=stub_->GetVerifyCode(&context,request,&reply);
	if(status.ok())
	{
		std::cout<<"GetVerifyCode Success"<<std::endl;
		return reply;
	}
	else
	{
		std::cout<<"GetVerifyCode Failed"<<std::endl;
		reply.set_error(ErrorCodes::RPCFailed);
		return reply;
	}
	return reply;
}
VerifyGrpcClient::VerifyGrpcClient()
{
	std::string ipPort=config["VerifyServer"]["ip"]+":"+config["VerifyServer"]["port"];
	std::cout<<"VerifyGrpcClient ipPort:"<<ipPort<<std::endl;
	std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(ipPort, grpc::InsecureChannelCredentials());
	
	stub_=message::VerifyService::NewStub(channel);
}
