#include "PerfmonWrapper.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <process.h>

using namespace std;

namespace libperfmon
{

PerfmonWrapper::PerfmonWrapper(void)
	: process_name_(L"")
	, log_name_(L"")
	, query_(nullptr)
	, pdh_status_(0)
	, counter_(nullptr)
	, log_type_(PDH_LOG_TYPE_BINARY)
	, log_(nullptr)
	, error_info_(ERROR_NONE)
	, args_(log_, error_info_)
{
}

PerfmonWrapper::~PerfmonWrapper(void)
{
	if(log_)
	{
		PdhCloseLog(log_, 0);
	}

	if(query_)
	{
		PdhCloseQuery(query_);
	}
}

bool PerfmonWrapper::Init(const WCHAR* process_name, const WCHAR* log_name)
{
	if(!_GetInitValue(process_name, log_name))
	{
		return false;
	}

	return true;
}

void PerfmonWrapper::SetWriteLogErrorCallback(WRITE_LOG_ERROR_CALLBACK func)
{
	error_call_back_ = func;
}

bool PerfmonWrapper::Start()
{
	if(!_OpenQuery())
	{
		return false;
	}

	if(!_AddCounters(_GetCounterCategory()))
	{
		return false;
	}

	if(!_OpenLog())
	{
		return false;
	}

	if(!_UpdateLog())
	{
		return false;
	}

	return true;
}

bool PerfmonWrapper::Stop()
{
	if(!TerminateThread(handle_, 0))
	{
		error_info_ = ErrorInfo(ERROR_STOP_THREAD_FAIL);
		return false;
	}
	return true;
}

const ErrorInfo PerfmonWrapper::GetErrorInfo() const
{
	return error_info_;
}

const wstring PerfmonWrapper::GetErrorString()
{
	wostringstream ostream;
	ostream << endl << "Error!!" << endl;
	ostream << "libperfmon Error Code - " << error_info_.error_code_ << endl;

	if(error_info_.pdh_status_ != 0)
	{
		ostream	<< "PDH Error Code - Ox" << hex << error_info_.pdh_status_ << endl;
	}

	return ostream.str();
}

bool PerfmonWrapper::_GetInitValue(const WCHAR* process_name, const WCHAR* log_name)
{
	if(process_name == nullptr || log_name == nullptr) 
	{
		error_info_ = ErrorInfo(ERROR_INVALID_INIT_VALUE);
		return false; 
	}

	process_name_	= process_name;
	log_name_		= log_name;

	return true;
}

bool PerfmonWrapper::_OpenQuery()
{
	//Creates a new query
	pdh_status_ = PdhOpenQuery(nullptr, 0, &query_);
	if(pdh_status_ != ERROR_SUCCESS)
	{
		error_info_ = ErrorInfo(ERROR_OPEN_QUERY_FAIL);
		return false;
	}

	return true;
}

bool PerfmonWrapper::_AddCounters(const wstring counter_category)
{
	//Add Counters
	wstring counter_path_arr[] = {L"\\% Processor Time"
								, L"\\% User Time"
								, L"\\% Privileged Time"
								, L"\\Thread Count"};
	for (int i = 0; i < _countof(counter_path_arr); i++)
	{
		wstring counter_path = counter_category + counter_path_arr[i];
		if(!_AddCounter(counter_path.c_str()))
		{
			return false;
		}
	}

	return true;
}

bool PerfmonWrapper::_AddCounter(const WCHAR* counter_path)
{
	// Adds the specified counter to the query.
	pdh_status_ = PdhAddCounter(query_, counter_path, 0, &counter_);
	if (pdh_status_ != ERROR_SUCCESS)
	{
		error_info_ = ErrorInfo(ERROR_ADD_COUNT_FAIL, pdh_status_);
		return false;
	}

	return true;
}

bool PerfmonWrapper::_OpenLog()
{
	//Opens the specified log file for writing.
	pdh_status_ = PdhOpenLog(_GetLogFileName().c_str(), PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS
		, &log_type_, query_, 0, nullptr, &log_);
	if (pdh_status_ != ERROR_SUCCESS)
	{
		//wprintf(L"PdhOpenLog failed with 0x%x\n", pdh_status_);
		error_info_ = ErrorInfo(ERROR_OPEN_LOG_FAIL);
		return false;
	}

	return true;
}

const wstring PerfmonWrapper::_GetCounterCategory()
{
	wstring counter_category = L"\\Process";
	counter_category += L"(" + process_name_ + L")";

	return counter_category;
}

const wstring PerfmonWrapper::_GetCurrentDate()
{
	time_t t = time(0);
	struct tm * now = localtime(&t);

	wostringstream ostream;
	ostream << (now->tm_year + 1900) << '_' 
		<< (now->tm_mon + 1) << '_'
		<< now->tm_mday << '_'
		<< now->tm_hour << '_'
		<< now->tm_min;

	return ostream.str();
}

const wstring PerfmonWrapper::_GetLogFileName()
{
	wstring file_ext = L".blg";
	return log_name_ + L"_" + _GetCurrentDate() + file_ext;
}

unsigned int CALLBACK WriteLog(void *arg) 
{
	if(arg == nullptr) { return 0; }

	ThreadArguments* args = static_cast<ThreadArguments*>(arg);
	while(true)
	{
		//Collects counter data for the current query and writes the data to the log file.
		PDH_STATUS  pdh_status = PdhUpdateLog (args->log_, nullptr);
		cout << "update log" << endl;
		if (ERROR_SUCCESS != pdh_status)
		{
			args->error_info_ = ErrorInfo(ERROR_CREATE_THREAD_FAIL, pdh_status);
			args->error_callback_(args->get_error_string_callback_());

			return 0;
		}

		Sleep(1000);
	}

	return 0;
}

bool PerfmonWrapper::_UpdateLog()
{
	unsigned int thread_id = 0;

	args_.log_						= log_;
	args_.error_info_				= error_info_;
	args_.get_error_string_callback_= std::bind(&PerfmonWrapper::GetErrorString, this);
	args_.error_callback_			= error_call_back_;

	handle_ = (HANDLE)_beginthreadex(nullptr, 0, &WriteLog, &args_, 0, &thread_id);
	if(handle_ == 0)
	{
		error_info_ = ErrorInfo(ERROR_CREATE_THREAD_FAIL);
		return false;
	}

	return true;
}

}

