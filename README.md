# :beer: libperfmon :ghost:
==========
![libperfmon](/libperfmon.png)  
libperfmon is a library for logging windows performance counter. 


## How to use
====================

This project is written in VS2010.

First, you should specify the counters that you want to check in 'CounterConfig.txt' in execute folder, like below
```bash
\Process(devenv)\% Processor Time
\Process(devenv)\Page Faults/sec
\Process(devenv)\IO Data Bytes/sec
\Process(devenv)\Working Set
\Process(devenv)\Thread Count
\Process(devenv)\Private Bytes
\Process(devenv)\Page File Bytes
\Process(devenv)\% Privileged Time
\Process(devenv)\% Processor Time
\Process(devenv)\% User Time
\Process(devenv)\Handle Count
```
You can also reference `ConterConfig_sample.txt` file in root folder.


## References
====================
 - http://www.appadmintools.com/documents/windows-performance-counters-explained/
 - http://msdn.microsoft.com/en-us/library/windows/desktop/aa373228(v=vs.85).aspx
 - http://askldjd.wordpress.com/2011/01/05/a-pdh-helper-class-cpdhquery/



