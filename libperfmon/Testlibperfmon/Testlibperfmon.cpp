// Testlibperfmon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "PerfmonWrapper.h"

#pragma comment(lib, "libperfmon.lib")

using namespace std;
using namespace libperfmon;

int wmain(int argc, WCHAR* argv[])
{
	//PerfmonWrapper init
	PerfmonWrapper wrapper;
	if(!wrapper.Init(L"Testlibperfmon"))
	{
		wcout << wrapper.GetErrorString().c_str() << endl;
		getchar();
		return -1;
	}
	wcout << L"init ok" << endl;

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

