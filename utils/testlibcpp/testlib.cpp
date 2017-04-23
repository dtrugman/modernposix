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

#include "testlib.hpp"

extern "C" Tester * create()
{
    return new Tester;
}

extern "C" void destroy(Tester * t)
{
    delete t;
}

Tester::Tester()
{
    // Do nothing
}

Tester::~Tester()
{
    // Do nothing
}

int Tester::add(int a, int b)
{
    return (a + b);
}