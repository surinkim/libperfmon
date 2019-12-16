#pragma once

#include <vector>
#include "CommonType.h"
#pragma comment(lib, "pdh.lib")

namespace libperfmon
{

class PerfmonWrapper
{
public:
	PerfmonWrapper(void);
	~PerfmonWrapper(void);

	bool Init(const WCHAR* process_name, const WCHAR* log_name = L"performance");
	void SetWriteLogErrorCallback(WRITE_LOG_ERROR_CALLBACK func);
	bool Start();
	bool Stop();
	const ErrorInfo GetErrorInfo() const;
	const wstring	GetErrorString();

private:
	bool _GetInitValue(const WCHAR* process_name, const WCHAR* log_name);
	bool _OpenQuery();
	bool _ReadCounterConfigFile(vector<wstring>& counters);
	bool _AddCounters(const vector<wstring>& counters, const wstring counter_category);
    bool _AddFixedCounters(const vector<wstring>& counters);
	bool _AddCounter(const WCHAR* counter_path);
	bool _OpenLog();
	bool _UpdateLog();
	
	const wstring _GetCounterCategory();
	const wstring _GetCurrentDate();
	const wstring _GetLogFileName();

	wstring			process_name_;
	wstring			log_name_;
	
	HQUERY			query_;
	PDH_STATUS		pdh_status_;
	HCOUNTER		counter_;
	DWORD			log_type_;
	HLOG			log_;

	HANDLE			handle_;
	ErrorInfo		error_info_;
	ThreadArguments args_;
	WRITE_LOG_ERROR_CALLBACK error_call_back_;
};

}