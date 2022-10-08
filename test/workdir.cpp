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

#include "modernposix/workdir.hpp"

#include "catch.hpp"

static const std::string NON_EXISTING_DIR("non_existing-dir");
static const std::string TEMP_DIR("/tmp"); // Reasonable to assume it's there

TEST_CASE("Working directory", "[workdir]")
{
    SECTION("Change to non-existing workdir")
    {
        REQUIRE_THROWS_AS(mp::workdir::chdir(NON_EXISTING_DIR), std::system_error);
    }

    SECTION("Change to existing workdir")
    {
        std::string initial_workdir = mp::workdir::getcwd();

        REQUIRE_NOTHROW(mp::workdir::chdir(TEMP_DIR));
        REQUIRE(mp::workdir::getcwd() == TEMP_DIR);

        REQUIRE_NOTHROW(mp::workdir::chdir(initial_workdir));
        REQUIRE(mp::workdir::getcwd() == initial_workdir);
    }
}

