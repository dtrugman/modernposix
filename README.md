# C++ Modern POSIX wrappers

## Motivation

Over the years, I have met many programmers that complain about using the POSIX API in CPP.

Every time they wrote a bunch of code to create a switch fabric between the API and their CPP code,
spending much time and resources on what should have been already there for them.

This led me into writing this library, so that anyone can have a much more intuitive API to the most common methods.

## Notes

**The wrapper classes do NOT provide additional thread-safety over the existing API**

## Process open (popen) wrapper

Following the CPP naming convention, I use the name pstream as in 'Process stream'.

We have two different pstreams, an input stream (`ipstream`) and an output stream (`opstream`).

The streams handle the underlying file descriptors and spares the hassle from you (the programmer).

### Input process stream (ipstream) example

The following snippet executes the `ls -l` command and stores the output into a vector of strings.

Find some working examples @ `examples/pstream.cpp`. Simple usage example:

```cpp
mp::ipstream reader("ls -l");
if (!reader)
{
    cerr << reader.error() << endl;
    return; // Or handle error otherwise
}

vector<string> output;
reader >> output;
```

Or, when using exceptions:

```cpp
try
{
    mp::ipstream reader("ls -l");

    vector<string> output;
    reader >> output;
}
catch (std::runtime_error & ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

### Output process stream (opstream) example

The following snippet executes the `wall` broadcast command and writes a message.

Upon destruction the writer closes the stream and flushes the content.

Find some working examples @ `examples/pstream.cpp`. Simple usage example:

```cpp
mp::opstream writer("wall");
if (!writer)
{
    cerr << writer.error() << endl;
    return; // Or handle error otherwise
}

// String splitted only as an example
writer << "testing modern output popen" << "\n";
```

Or, when using exceptions:

```cpp
try
{
    mp::opstream writer("wall");

    // String splitted only as an example
    writer << "testing modern output popen" << "\n";
}
catch (std::runtime_error & ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

## Dynamic library wrapper

This class wraps `dlopen()`, `dlsym()`, `dlclose()` & `dlerror()` APIs, allowing easy access to dynamic libraries.

The class assures that the library is closed upon destruction.

You can also specify the type of initilization you require upon construction, default is LAZY and LOCAL, see `man dlopen(3)` for more information.

Find some working examples @ `examples/dynamiclib.cpp`. Simple usage example:

```cpp
mp::dynamiclib dlib(TEST_LIB_C);
if (!dlib)
{
    cerr << dlib.error() << endl;
    return; // Or handle error otherwise
}

Multiply multiply = (Multiply)dlib.symbol(TEST_LIB_C_MULTIPLY);
if (!multiply)
{
    cerr << dlib.error() << endl;
    return; // Or handle error otherwise
}

multiply(a,b);
```

Or, when using exceptions:

```cpp
try
{
    mp::dynamiclib dlib(TEST_LIB_C);
    Multiply multiply = (Multiply)dlib.symbol(TEST_LIB_C_MULTIPLY);
    multiply(a,b);
}
catch (std::runtime_error & ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

## Directory stream wrapper

This class simply wraps the `opendir()`, `readdir()`, `rewinddir()` & `closedir()` APIs, allowing easy access to directory entry enumeration.

It assures proper handling of the underlying resources upon destruction.

Find some working examples @ `examples/dstream.cpp`. Simple usage example:

```cpp
mp::idstream ds(TEST_DIR);
if (!ds)
{
    cerr << ds.error() << endl;
    return; // Or handle error otherwise
}

set<string> output;
ds >> output;
```

Or, when using exceptions:

```cpp
try
{
    mp::idstream ds(TEST_DIR);
    set<string> output;
    ds >> output;
}
catch (std::runtime_error & ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

## Working directory wrapper

This class wraps `getcwd` and `chdir` so it will be possible to use them directly with std::string variables.

Again, this wrapper helps us avoid raw cstrings in our program.

Find a working example @ `examples/workdir.cpp`. Simple usage example:

```cpp
string initial_workdir;
if (!mp::workdir::getcwd(initial_workdir))
{
    cerr << mp::workdir::error() << endl;
    return; // Or handle error otherwise
}

string new_workdir("/tmp")
if (!mp::workdir::chdir(new_workdir))
{
    cerr << mp::workdir::error() << endl;
    return; // Or handle error otherwise
}

// Do stuff...

if (!mp::workdir::chdir(initial_workdir))
{
    cerr << mp::workdir::error() << endl;
    return; // Or handle error otherwise
}
```

Or, when using exceptions:

```cpp
try
{
    string initial_workdir;
    mp::workdir::getcwd(initial_workdir);

    string new_workdir("/tmp")
    mp::workdir::chdir(new_workdir);

    // Do stuff...

    mp::workdir::chdir(initial_workdir);
}
catch (std::runtime_error & ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

## Installation

Just add the files from the include directory to your project and compile.

## Examples

If you wish to compile this project and try out the included examples, just run:

```bash
scons
./examples/<specific-example>
```

## Tests

If you wish to compile this project and run the included unit-tests, just run:

```bash
scons
scons test
```

## License

Copyright 2018 Daniel Trugman

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
