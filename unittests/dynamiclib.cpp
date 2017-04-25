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

#include "catch.hpp"
#include "utils.hpp"

#include "../include/dynamiclib.hpp"

#include "../utils/testlibc/testlib.h"
#include "../utils/testlibcpp/testlib.hpp"

#include <unistd.h>

using namespace std;

static const char * NON_EXISTING_LIB = "libnonexisting.so";

static const char * TEST_LIB_C = "utils/testlibc/libtestlib.so";
static const char * TEST_LIB_CPP = "utils/testlibcpp/libtestlib.so";

static const char * NON_EXISTING_SYMBOL = "badsymbol";

TEST_CASE("Dynamic library", "[dynamiclib]")
{
    SECTION("Load non-existing library")
    {
        REQUIRE_INIT_FAILURE(mp::dynamiclib, std::runtime_error, NON_EXISTING_LIB);
    }

    SECTION("C library")
    {
        mp::dynamiclib dlib(TEST_LIB_C);
        REQUIRE(dlib);

        SECTION("Load unexisting")
        {
            Multiply multiply;
            REQUIRE_THROWS_AS(multiply = (Multiply)dlib.symbol(NON_EXISTING_SYMBOL), std::runtime_error);
        }

        SECTION("Use")
        {            
            Multiply multiply;
            REQUIRE_NOTHROW(multiply = (Multiply)dlib.symbol("multiply"));
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

        SECTION("Load unexisting")
        {
            Create create;
            REQUIRE_THROWS_AS(create = (Create)dlib.symbol(NON_EXISTING_SYMBOL), std::runtime_error);
        }

        SECTION("Use")
        {
            Create create;
            REQUIRE_NOTHROW(create = (Create)dlib.symbol("create"));
            REQUIRE(create != NULL);

            Destroy destroy;
            REQUIRE_NOTHROW(destroy = (Destroy)dlib.symbol("destroy"));
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

