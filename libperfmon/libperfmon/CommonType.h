#pragma once

#include <Windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <functional>

using namespace std;

namespace libperfmon
{

typedef void (CALLBACK *WRITE_LOG_ERROR_CALLBACK)(const wstring error_msg);
typedef std::function<const wstring()> GET_ERROR_STRING_CALLBACK;

enum ErrorCode
{
	ERROR_NONE						= 0,
	ERROR_INVALID_INIT_VALUE		= 1,
	ERROR_NO_COUNTER_CONFIG_FILE	= 2,
	ERROR_OPEN_QUERY_FAIL			= 3,
	ERROR_ADD_COUNT_FAIL			= 4,
	ERROR_OPEN_LOG_FAIL				= 5,
	ERROR_UPDATE_LOG_FAIL			= 6,
	ERROR_CREATE_THREAD_FAIL		= 7,
	ERROR_STOP_THREAD_FAIL			= 8,
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
	ThreadArguments(HLOG& log, ErrorInfo& error_info) 
		: log_(log), error_info_(error_info) {}

	HLOG&						log_;
	ErrorInfo&					error_info_;

	GET_ERROR_STRING_CALLBACK	get_error_string_callback_;
	WRITE_LOG_ERROR_CALLBACK	error_callback_;
};

}