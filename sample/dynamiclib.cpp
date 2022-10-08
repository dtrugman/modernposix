/*
 *  Copyright 2017-present Daniel Trugman
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdio.h>

#include <iostream>

#include "modernposix/dynamiclib.hpp"

#include "lib/c/lib.h"
#include "lib/cpp/lib.hpp"

static const char * C_LIB = "lib/libtest_c.so";
static const char * C_MULTIPLY = "multiply";

static const char * CPP_LIB = "lib/libtest_cpp.so";
static const char * CPP_CREATE = "create";
static const char * CPP_DESTROY = "destroy";

void dynamiclibc(int x, int y)
{
    mp::dynamiclib dlib(C_LIB);
    multiply_fp multiply = (multiply_fp)dlib.symbol(C_MULTIPLY);
    std::cout << "x * y = " << multiply(x, y) << std::endl;
}

void dynamiclibcpp(int x, int y)
{
    mp::dynamiclib dlib(CPP_LIB);

    create_fp create = (create_fp)dlib.symbol(CPP_CREATE);
    destroy_fp destroy = (destroy_fp)dlib.symbol(CPP_DESTROY);

    tester* tester = create();
    std::cout << "x + y = " << tester->add(x, y) << std::endl;
    destroy(tester);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <x> <y>" << std::endl;
        return 2;
    }
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    dynamiclibc(x, y);
    dynamiclibcpp(x, y);

    return 0;
}
