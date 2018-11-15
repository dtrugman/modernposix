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

#include "../include/workdir.hpp"

using namespace std;

static const char * BAD_WORKDIR = "/nonexisting_tempdir";
static const char * VALID_WORKDIR = "/tmp";

TEST_CASE("Working directory", "[workdir]")
{
    SECTION("Change to non-existing workdir")
    {
        REQUIRE_CALL_FAILURE(bool, false, mp::workdir::chdir,
                             std::runtime_error, BAD_WORKDIR);
    }

    SECTION("Change to existing workdir")
    {
        std::string initial_workdir;
        std::string current_workdir;

        REQUIRE(mp::workdir::getcwd(initial_workdir));
        
        REQUIRE(mp::workdir::chdir(VALID_WORKDIR));
        REQUIRE(mp::workdir::getcwd(current_workdir));
        REQUIRE(current_workdir == VALID_WORKDIR);

        REQUIRE(mp::workdir::chdir(initial_workdir));
        REQUIRE(mp::workdir::getcwd(current_workdir));
        REQUIRE(current_workdir == initial_workdir);
    }
}

