// Testlibperfmon.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "PerfmonWrapper.h"

#pragma comment(lib, "libperfmon.lib")

using namespace std;
using namespace libperfmon;

PerfmonWrapper wrapper;
void CALLBACK WriteLogErrorCallback(const wstring error_msg)
{
	wcout << L"ERROR!! libperfmon write log " << endl;
	wcout << error_msg.c_str() << endl;
}

int wmain(int argc, WCHAR* argv[])
{
	//PerfmonWrapper init
	if(!wrapper.Init(L"Testlibperfmon"))
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

