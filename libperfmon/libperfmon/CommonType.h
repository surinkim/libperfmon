#pragma once

#include <Windows.h>
#include <pdh.h>
#include <pdhmsg.h>

namespace libperfmon
{

enum ErrorCode
{
	ERROR_NONE,
	ERROR_INVALID_INIT_VALUE,
	ERROR_OPEN_QUERY_FAIL,
	ERROR_ADD_COUNT_FAIL,
	ERROR_OPEN_LOG_FAIL,
	ERROR_UPDATE_LOG_FAIL,
	ERROR_CREATE_THREAD_FAIL,
	ERROR_STOP_THREAD_FAIL,
};

struct ErrorInfo
{
	ErrorInfo(const ErrorCode error_code) 
		: error_code_(error_code), pdh_status_(0) {}
	ErrorInfo(const ErrorCode error_code, const PDH_STATUS pdh_status) 
		: error_code_(error_code), pdh_status_(pdh_status) {}

	ErrorCode	error_code_;
	PDH_STATUS	pdh_status_;
};

struct ThreadArguments
{
	ThreadArguments(const HLOG log, const ErrorInfo error_info) 
		: log_(log), error_info_(error_info) {}

	HLOG		log_;
	ErrorInfo	error_info_;
};

}