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

#include "catch.hpp"

#include "../include/dynamiclib.hpp"

#include "utils.hpp"

#include "../utils/testlibcpp/testlib.hpp"

#include <unistd.h>

using namespace std;

static const char * NON_EXISTING_LIB = "libnonexisting.so";
static const char * TEST_LIB_C = "utils/testlibc/libtestlib.so";
static const char * TEST_LIB_CPP = "utils/testlibcpp/libtestlib.so";

TEST_CASE("Dynamic library", "[dynamiclib]")
{
    SECTION("Load non-existing library")
    {
        REQUIRE_FAILURE(mp::dynamiclib dlib(NON_EXISTING_LIB), std::runtime_error);
    }

    SECTION("C library")
    {
        mp::dynamiclib dlib(TEST_LIB_C);
        REQUIRE(dlib);

        SECTION("Use")
        {
            typedef int (*Multiply)(int, int);
            
            Multiply multiply = (Multiply)dlib.symbol("multiply");

            REQUIRE(multiply != NULL);
            REQUIRE(multiply(2,3) == 6);
        }

        SECTION("Close loaded")
        {
            REQUIRE(dlib.close());
        }

        SECTION("Close unloaded")
        {
            REQUIRE(dlib.close());
            // The library should be closed by now
            REQUIRE(dlib.close());
        }
    }

    SECTION("CPP library")
    {
        mp::dynamiclib dlib(TEST_LIB_CPP);
        REQUIRE(dlib);

        SECTION("Use")
        {
            typedef Tester * (*Create)();
            typedef void (*Destroy)(Tester *);

            Create create = (Create)dlib.symbol("create");
            REQUIRE(create != NULL);

            Destroy destroy = (Destroy)dlib.symbol("destroy");
            REQUIRE(destroy != NULL);
            
            Tester * tester = create();
            REQUIRE(tester != NULL);

            REQUIRE(tester->add(2,3) == 5);

            REQUIRE_NOTHROW(destroy(tester));
        }

        SECTION("Close loaded")
        {
            REQUIRE(dlib.close());
        }
        
        SECTION("Close unloaded")
        {
            REQUIRE(dlib.close());
            // The library should be closed by now
            REQUIRE(dlib.close());
        }
    }
}

