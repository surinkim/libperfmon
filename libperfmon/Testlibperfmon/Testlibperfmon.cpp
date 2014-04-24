// Testlibperfmon.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "PerfmonWrapper.h"

#pragma comment(lib, "libperfmon.lib")

using namespace std;
using namespace libperfmon;

wstring GetProcessName(WCHAR* first_argv);
void CALLBACK WriteLogErrorCallback(const wstring error_msg);

int wmain(int argc, WCHAR* argv[])
{
	//Get process name
	wstring process_name = GetProcessName(argv[0]);

	//PerfmonWrapper init
	PerfmonWrapper wrapper;
	if(!wrapper.Init(process_name.c_str()))
	{
		wcout << wrapper.GetErrorString().c_str() << endl;
		getchar();
		return -1;
	}
	wcout << L"init ok" << endl;

	//Set WriteLogErrorCallback
	wrapper.SetWriteLogErrorCallback(WriteLogErrorCallback);

	//start writing performance log
	if(!wrapper.Start())
	{
		wcout << wrapper.GetErrorString().c_str() << endl;
		getchar();
		return -1;
	}
	wcout << L"start ok" << endl;

	//do something long work for test
	for(int i = 0; i < 10000; i++)
	{
		int j = i / 2371;
		Sleep(100);
	}

	//stop writing performance log
	if(!wrapper.Stop())
	{
		cout << wrapper.GetErrorString().c_str() << endl;
		getchar();
		return -1;
	}
	wcout << L"stop" << endl;

	getchar();
	return 0;
}

wstring GetProcessName(WCHAR* first_argv)
{
	if(first_argv == nullptr)
	{
		return false;
	}

	wstring s(first_argv);
	wstring delimiter = L"\\";

	size_t pos = 0;
	wstring token;
	while ((pos = s.find(delimiter)) != std::wstring::npos) {
		s.erase(0, pos + delimiter.length());
	}
	return s;
}

void CALLBACK WriteLogErrorCallback(const wstring error_msg)
{
	wcout << L"ERROR!! libperfmon write log " << endl;
	wcout << error_msg.c_str() << endl;
}