# Pure C library for IRBIS64 system

Universal client software for IRBIS64 library automation system (ManagedClient ported to plain C90). 

### Features

* Contains no references for external components. Doesn't require `irbis64_client.dll`.
* Can be compiled as static library (`.lib` or `.a`) and as dynamic library (`.dll` or `.so`).
* Supports both simple and complex search for database records with optional formatting.
* Supports records creation and/or modification. Database records can be updated too. 
* Support for database dictionary listing for terms and postings. 
* Administrator functions: get list of (active) users, create/delete user, create/delete database and so on. 

### Supported platforms

* 32-bit and 64-bit Windows (XP, 7, 8, 8.1, 10);
* MacOS X 10.14 (64-bit);
* 32-bit and 64-bit Linux (CentOS 7/8);
* 64-bit FreeBSD (starting with version 11);

### Supported compilers

* MSVS 2013/2015/2017/2019 Community Edition on Windows;
* GCC 4.8 or higher both on Windows (MinGW) and Linux/OS X;
* Clang compiler 9 both on Windows and Linux;
* Intel C compiler 19 on Windows, Linux/OS X.

### Simple example

```c
#include "magna/irbis.h"

int main (int argc, char **argv)
{
    Connection connection;
    am_mfn maxMfn;

    printf ("Library version: %d\n", magna_version());

    connection_init (&connection);
    connection_set_host     (&connection, "localhost");
    connection_set_username (&connection, "librarian");
    connection_set_password (&connection, "secret");
    connection_set_databasE (&connection, "IBIS");
    connection.workstation = CATALOGER;
    connection.port = 6666;

    if (!irbis_connect (&connection)) {
        fputs ("Connection failed", stderr);
        connection_destroy (&connection);
        return 1;
    }

    maxMfn = connection_get_max_mfn (&connection, NULL);
    printf ("Max MFN=%u\n", maxMfn);

    irbis_disconnect (&connection);
    connection_destroy (&connection);

    return 0;
}
```

### Build status

[![Issues](https://img.shields.io/github/issues/amironov73/PlainIrbis.svg)](https://github.com/amironov73/PlainIrbis/issues)
[![Build status](https://img.shields.io/appveyor/ci/AlexeyMironov/plainirbis.svg)](https://ci.appveyor.com/project/AlexeyMironov/plainirbis/)
[![Build status](https://api.travis-ci.org/amironov73/PlainIrbis.svg)](https://travis-ci.org/amironov73/PlainIrbis/)

#### Documentation (in russian)

* [**Introduction**](docs/chapter1.md)
* [**Connection structure**](docs/chapter2.md)
* [**MarcRecord, MarcField and SubField structures**](docs/chapter3.md)
