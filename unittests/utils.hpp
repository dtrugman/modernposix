/*
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
*/

#ifndef MP_UNITTEST_UTILS_HPP
#define MP_UNITTEST_UTILS_HPP

#include "../include/modernposix.hpp"

#define LENGTHOF(x) (sizeof(x)/sizeof((x)[0]))

// REQUIRE_INIT_FAILURE definition
// Tests a bad initialization of an MP class
// When configured to throw exceptions, expects an exception
// otherwise, expects NO exceptions and tests the class' bool() operator

#ifdef MP_NO_THROW
    #define REQUIRE_INIT_FAILURE(type, ex, ...) \
        type * t = NULL; \
        REQUIRE_NOTHROW(t = new type(__VA_ARGS__)); \
        REQUIRE_FALSE(*t); \
        delete t;
#else // MP_NO_THROW
    #define REQUIRE_INIT_FAILURE(type, ex, ...) \
        type * t = NULL; \
        REQUIRE_THROWS_AS(t = new type(__VA_ARGS__), ex); \
        delete t;
#endif // MP_NO_THROW

#endif // MP_UNITTEST_UTILS_HPP