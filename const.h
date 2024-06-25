#pragma once

class ConfigMgr;
extern ConfigMgr config;
enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002
};