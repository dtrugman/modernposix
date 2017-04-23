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

using namespace std;

static const char * TEST_LIB_C = "utils/testlibc/libtestlib.so";

void dynamiclib()
{
    try
    {
        mp::dynamiclib dlib(TEST_LIB_C);
        
        cout << "Successfully loaded library [" << TEST_LIB_C << "]" << endl;

        typedef int (*Multiply)(int, int);    
        Multiply multiply = (Multiply)dlib.symbol("multiply");
        if (multiply == NULL)
        {
            throw std::runtime_error(dlib.error());
        }

        int a, b;

        cout << "Please insert first number: ";
        cin >> a;
        cout << "Please insert second number: ";
        cin >> b;
        cout << "Result is: " << multiply(a,b) << endl;
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

    dynamiclib();

    return 0;
}
