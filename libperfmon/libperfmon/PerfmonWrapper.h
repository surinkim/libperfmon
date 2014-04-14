#pragma once

#include <string>

#include "CommonType.h"

#pragma comment(lib, "pdh.lib")

using namespace std;

namespace libperfmon
{

class PerfmonWrapper
{
public:
	PerfmonWrapper(void);
	~PerfmonWrapper(void);

	bool Init(const WCHAR* process_name, const WCHAR* log_name = L"performance");
	bool Start();
	bool Stop();
	const ErrorInfo GetErrorInfo() const;
	const wstring	GetErrorString();

private:
	bool _GetInitValue(const WCHAR* process_name, const WCHAR* log_name);
	bool _OpenQuery();
	bool _AddCounters(const wstring counter_category);
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
};

}