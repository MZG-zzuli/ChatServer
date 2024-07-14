#pragma once


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002, 
	VerifyExpired=1004,	// 验证码过期
	VerifyCodeErr=1005,	// 验证码错误
	UserExist=1006		// 用户已存在
};