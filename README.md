![mp](./img/mp.png "Logo")

Header-only C++ wrappers for POSIX APIs.

## Build

![Build & Test](https://github.com/dtrugman/modernposix/actions/workflows/cmake.yml/badge.svg)

This is a header-only library. Copy the `modernposix` directory from under `include` into your project's include directory.

Compile samples and unittests using `cmake . && make`

## Requirements

- The library requires C++11 or newer

## Notes

- All APIs and function calls do NOT provide additional thread-safety over the existing API

## Process open (popen) wrapper

Following the CPP naming convention, I use the name pstream as in 'Process stream'.

We have two different pstreams, an input stream (`ipstream`) and an output stream (`opstream`).

The streams handle the underlying file descriptors and spares the hassle from you (the programmer).

### Input process stream (ipstream) example

The following snippet executes the `ls -l` command and stores the output into a vector of strings:

```cpp
try
{
    mp::ipstream reader("ls -l");

    vector<string> output;
    reader >> output;
}
catch (std::runtime_error& ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

### Output process stream (opstream) example

The following snippet executes the `wall` broadcast command and writes a message.
Upon destruction the writer closes the stream and flushes the content:

```cpp
try
{
    mp::opstream writer("wall");
    writer << "WOW! This library rocks";
}
catch (std::runtime_error& ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

## Dynamic library wrapper

This class wraps `dlopen()`, `dlsym()`, `dlclose()` & `dlerror()` APIs, allowing easy access to dynamic libraries.

The class assures that the library is closed upon destruction.

You can also specify the type of initilization you require upon construction, default is LAZY and LOCAL, see `man dlopen(3)` for more information.

The following snippet loads a shared library written in C and calls one of its exported symbols:

```cpp
try
{
    mp::dynamiclib dlib(TEST_LIB_C);
    multiply_fp multiply = static_cast<multiply_fp>(dlib.symbol(TEST_LIB_C_MULTIPLY));
    multiply(a,b);
}
catch (std::runtime_error& ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```

## Directory stream wrapper

This class simply wraps the `opendir()`, `readdir()`, `rewinddir()` & `closedir()` APIs, allowing easy access to directory entry enumeration.

The following snippet reads all the entries in a given directory:

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

This snippet shows how an application can change its working directory just for a few commands. When you leave the scope, it will change the directory straight back.

You can also use `mp::workdir::chdir` directly if you want.

```cpp
try
{
    string initial_workdir;
    mp::workdir::getcwd(initial_workdir);

    mp::workdir::pushd("/tmp");

    // Do stuff...
}
catch (std::runtime_error & ex)
{
    // Something went wrong and ex.what() will tell you what it is
}
```
