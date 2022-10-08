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

#include "modernposix/last_error.hpp"

#include "catch.hpp"

static const std::string SUCCESS_STR("Success");
static const std::string INVALID_ARGUMENT_STR("Invalid argument");

TEST_CASE("Last error get", "[last_error]")
{
    int error;

    SECTION("Sanity negative value")
    {
        error = -EINVAL;
    }

    SECTION("Sanity positive value")
    {
        error = EINVAL;
    }

    errno = error;
    REQUIRE(mp::last_error::is_set());
    REQUIRE(mp::last_error::get() == error);
}

TEST_CASE("Last error clear", "[last_error]")
{
    errno = EINVAL;
    REQUIRE(mp::last_error::is_set());

    mp::last_error::clear();
    REQUIRE(!mp::last_error::is_set());
    REQUIRE(mp::last_error::get() == 0);
}

TEST_CASE("Last error to string", "[last_error]")
{
    SECTION("No error")
    {
        errno = 0;
        REQUIRE(mp::last_error::to_string() == SUCCESS_STR);
    }

    SECTION("Some error")
    {
        errno = EINVAL;
        REQUIRE(mp::last_error::to_string() == INVALID_ARGUMENT_STR);
    }
}

TEST_CASE("Last error to exception", "[last_error]")
{
    SECTION("Some error")
    {
        std::string prefix("Hi");

        errno = EINVAL;
        std::system_error ex = mp::last_error::to_exception(prefix);
        REQUIRE(ex.what() == prefix + ": " + INVALID_ARGUMENT_STR);
    }
}
