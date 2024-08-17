#pragma once


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002,
	VerifyExpired = 1003,	// 验证码过期
	VerifyCodeErr = 1004,	// 验证码错误
	UserExist = 1005,		// 用户已存在
	PasswdErr = 1006,		// 密码错误
	UserError = 1007,
	RPCGetFailed=1008,
};