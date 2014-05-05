libperfmon
==========

libperfmon is a library for logging windows performance counter. 


How to use
====================

This project is written in VS2010.

First, you should specify the counters that you want to check in 'CounterConfig.txt'.
Then, you can use the library like below,

```c++
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

```

Current version can only support related in 'process' category.

References
====================

 - http://msdn.microsoft.com/en-us/library/windows/desktop/aa373228(v=vs.85).aspx
 - http://askldjd.wordpress.com/2011/01/05/a-pdh-helper-class-cpdhquery/
 

Author
====================

 - web : http://attractiveapproach.blogspot.com  
 - mail : nnhope@hotmail.com  
 - stackoverflow : http://stackoverflow.com/users/2231098/codedreamer


