# C++ Modern POSIX wrappers

## Motivation

Over the years, I have met many programmers that complain about using the POSIX API in CPP.

Every time they tried to integrate it into their projects the results were real nasty.

This led me into writing this library, so that anyone can have a much more intuitive API to the most common methods.

## Notes

**The wrapper classes do NOT provide additional thread-safety over the existing API**

## Process open (popen) wrapper

Following the CPP naming convention, I use the name pstream as in 'Process stream'.

We have two different pstreams, an input stream (ipstream) and an output stream (opstream).

The streams handle the underlying file descriptors and spare the hassle from you (the programmer).

### Input process stream (ipstream) example

The following snippet executes the 'ls -l' command and stores the output into a vector of strings.

```cpp
mp::ipstream reader("ls -l");

vector<string> output;
reader >> output;
```

### Output process stream (opstream) example

The following snippet executes the 'wall' broadcast command and writes a message.

Upon destruction the writer closes the stream and flushes the content.

```cpp
mp::opstream writer("wall");

writer << "testing" << " " << "modern output popen" << "\n";
```

## Dynamic library wrapper

This class simply wraps the dlopen(), dlsym(), dlclose() & dlerror() APIs, allowing easy access to dynamic libraries.

The class assures that the library is closed upon destruction.

You can also specify the type of initilization you require upon construction, default is LAZY and LOCAL, see `man dlopen(3)` for more information.

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

Copyright 2017 Daniel Trugman

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
