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

#ifndef MP_UNITTESTS_UTILS_HPP
#define MP_UNITTESTS_UTILS_HPP

#include "../include/config.hpp"

#include "catch.hpp"

#ifdef MP_NO_THROW
    #define REQUIRE_INIT_FAILURE(ex, type, name, ...) \
        try { \
            type name(__VA_ARGS__); \
            REQUIRE(!name); \
        } catch(ex & e) { \
            REQUIRE(false); \
        }
#else // MP_NO_THROW
    #define REQUIRE_INIT_FAILURE(ex, type, name, ...) \
        try { \
            type name(__VA_ARGS__); \
            REQUIRE(false); \
        } catch(ex & e) { \
            REQUIRE(true); \
        }
#endif // MP_NO_THROW

#endif // MP_UNITTESTS_UTILS_HPP