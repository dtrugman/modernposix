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

#include "modernposix/dynamiclib.hpp"

#include "catch.hpp"

static const std::string INEXISTING_LIB("libtest_inexistent.so");

static const std::string INEXISTING_ERR = INEXISTING_LIB +
     ": cannot open shared object file: No such file or directory";

TEST_CASE("Dynamic loader last error get", "[last_error]")
{
    try
    {
        mp::dynamiclib lib(INEXISTING_LIB);
    }
    catch (...)
    {
        // Do nothing
    }

    SECTION("Get works")
    {
        char *err = mp::dl_last_error::get();
        REQUIRE(err != NULL);
        REQUIRE(std::string(err) == INEXISTING_ERR);
    }

    SECTION("Get to string works")
    {
        std::string err = mp::dl_last_error::to_string();
        REQUIRE(err == INEXISTING_ERR);
    }

    SECTION("Get to exception works")
    {
        std::string prefix("Hi");
        std::runtime_error err = mp::dl_last_error::to_exception(prefix);
        REQUIRE(err.what() == prefix + ": " + INEXISTING_ERR);
    }
}

TEST_CASE("Dynamic loader last error clearing", "[last_error]")
{
    try
    {
        mp::dynamiclib lib(INEXISTING_LIB);
    }
    catch (...)
    {
        // Do nothing
    }

    mp::dl_last_error::clear();

    char *err = mp::dl_last_error::get();
    REQUIRE(err == NULL);
}

TEST_CASE("Dynamic loader last error to string always safe", "[last_error]")
{
    mp::dl_last_error::clear();

    REQUIRE_NOTHROW(mp::dl_last_error::to_string());
    REQUIRE_NOTHROW(mp::dl_last_error::to_string());
}
