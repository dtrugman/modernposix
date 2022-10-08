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

#include <unistd.h>

#include "modernposix/dynamiclib.hpp"

#include "lib/c/lib.h"
#include "lib/cpp/lib.hpp"

#include "catch.hpp"

static const char* INEXISTING_SYMBOL = "badsymbol";
static const char* INEXISTING_LIB = "libtest_inexistent.so";

static const char* C_LIB = "lib/libtest_c.so";
static const char* C_MULTIPLY = "multiply";

static const char* CPP_LIB = "lib/libtest_cpp.so";
static const char* CPP_CREATE = "create";
static const char* CPP_DESTROY = "destroy";

TEST_CASE("Dynamic library", "[dynamiclib]")
{
    SECTION("Load non-existing library")
    {
        REQUIRE_THROWS_AS(mp::dynamiclib(INEXISTING_LIB), std::runtime_error);
    }

    SECTION("C library")
    {
        mp::dynamiclib dlib(C_LIB);

        SECTION("Load inexisting")
        {
            REQUIRE_THROWS_AS(dlib.symbol(INEXISTING_SYMBOL), std::runtime_error);
        }

        SECTION("Use")
        {
            multiply_fp multiply = (multiply_fp)dlib.symbol(C_MULTIPLY);
            REQUIRE(multiply != NULL);
            REQUIRE(multiply(2, 3) == 6);
        }

        SECTION("Close loaded")
        {
            REQUIRE_NOTHROW(dlib.close());
        }

        SECTION("Close unloaded")
        {
            REQUIRE_NOTHROW(dlib.close());
            // The library should be closed by now
            REQUIRE_NOTHROW(dlib.close());
        }
    }

    SECTION("CPP library")
    {
        mp::dynamiclib dlib(CPP_LIB);

        SECTION("Load unexisting")
        {
            REQUIRE_THROWS_AS(dlib.symbol(INEXISTING_LIB), std::runtime_error);
        }

        SECTION("Use")
        {
            create_fp create = (create_fp)dlib.symbol(CPP_CREATE);
            REQUIRE(create != NULL);

            destroy_fp destroy = (destroy_fp)dlib.symbol(CPP_DESTROY);
            REQUIRE(destroy != NULL);

            tester* tester = create();
            REQUIRE(tester != NULL);
            REQUIRE(tester->add(2, 3) == 5);

            REQUIRE_NOTHROW(destroy(tester));
        }

        SECTION("Close loaded")
        {
            REQUIRE_NOTHROW(dlib.close());
        }

        SECTION("Close unloaded")
        {
            REQUIRE_NOTHROW(dlib.close());
            // The library should be closed by now
            REQUIRE_NOTHROW(dlib.close());
        }
    }
}

