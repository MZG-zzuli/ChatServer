#pragma once


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002,
	VerifyExpired = 1003,	// ��֤�����
	VerifyCodeErr = 1004,	// ��֤�����
	UserExist = 1005,		// �û��Ѵ���
	PasswdErr = 1006,		// �������
	UserError = 1007,
	RPCGetFailed=1008,
};