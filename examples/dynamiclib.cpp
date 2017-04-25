/*
    Copyright 2016 Daniel Trugman

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <iostream>

#include "../include/dynamiclib.hpp"

#include "../utils/testlibc/testlib.h"
#include "../utils/testlibcpp/testlib.hpp"

using namespace std;

static const char * TEST_LIB_C = "utils/testlibc/libtestlib.so";
static const char * TEST_LIB_C_MULTIPLY = "multiply";

static const char * TEST_LIB_CPP = "utils/testlibcpp/libtestlib.so";
static const char * TEST_LIB_CPP_CREATE = "create";
static const char * TEST_LIB_CPP_DESTROY = "destroy";

void dynamiclibc()
{
    try
    {
        mp::dynamiclib dlib(TEST_LIB_C);

        // Multiply type defined in lib header file
        Multiply multiply = (Multiply)dlib.symbol(TEST_LIB_C_MULTIPLY);

        int a, b;
        cout << "Please enter two numbers: ";
        cin >> a >> b;
        cout << "The result (a*b) is: " << multiply(a,b) << endl;
    }
    catch(std::runtime_error & err)
    {
        cout << err.what() << endl;
    }
}

void dynamiclibcpp()
{
    try
    {
        mp::dynamiclib dlib(TEST_LIB_CPP);

        // Create & Destroy types defined in lib header file
        Create create = (Create)dlib.symbol(TEST_LIB_CPP_CREATE);
        Destroy destroy = (Destroy)dlib.symbol(TEST_LIB_CPP_DESTROY);

        Tester * tester = create();

        int a, b;
        cout << "Please enter two numbers: ";
        cin >> a >> b;
        cout << "The result (a+b) is: " << tester->add(a,b) << endl;

        destroy(tester);
    }
    catch(std::runtime_error & err)
    {
        cout << err.what() << endl;
    }
}

int main()
{
    // NOTE! These examples assume that the module is compiled to throw exceptions
    // upon error conditions during initialization!

    dynamiclibc();
    dynamiclibcpp();

    return 0;
}
