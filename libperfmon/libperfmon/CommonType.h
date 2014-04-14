#pragma once

#include <Windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>

using namespace std;

namespace libperfmon
{

typedef void (CALLBACK *WRITE_LOG_ERROR_CALLBACK)(
	/*__inout     PTP_CALLBACK_INSTANCE Instance,
	__inout_opt PVOID                 Context,
	__inout     PTP_WORK              Work*/

	const wstring error_msg
	);

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
	WRITE_LOG_ERROR_CALLBACK error_call_back_;
};

}