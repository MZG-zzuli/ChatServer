#pragma once


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002, 
	VerifyExpired=1004,	// ��֤�����
	VerifyCodeErr=1005,	// ��֤�����
	UserExist=1006		// �û��Ѵ���
};