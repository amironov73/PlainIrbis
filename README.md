# Pure C library for IRBIS64 system

Universal client software for IRBIS64 library automation system (ManagedClient ported to plain C99). 

### Features

* Contains no references for external components. Doesn't require `irbis64_client.dll`.
* Can be compiled as static library (`.lib` or `.a`) and as dynamic library (`.dll` or `.so`).
* Supports both simple and complex search for database records with optional formatting.
* Supports records creation and/or modification. Database records can be updated too. 
* Support for database dictionary listing for terms and postings. 
* Administrator functions: get list of (active) users, create/delete user, create/delete database and so on. 

### Supported platforms and compilers

* 32-bit and 64-bit Windows (XP, 7, 8, 8.1, 10);
* MacOS X 10.14 (64-bit);
* 32-bit and 64-bit Linux (CentOS 7/8);
* 64-bit FreeBSD (starting with version 11);
* MSVS 2015/2017/2019 Community Edition on Windows;
* CMake 2.8 or higher on Linux.
* GCC 4.8 or higher both on Windows (MinGW) and Linux/OS X;
* Clang compiler 9 both on Windows and Linux;
* Intel C compiler 19 on Windows, Linux/OS X.
