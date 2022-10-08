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

#include <set>
#include <vector>

#include "modernposix/dstream.hpp"

#include "catch.hpp"
#include "autogen_dir.hpp"

static const std::string NON_EXISTING_DIR("non_existing-dir");

TEST_CASE("Directory stream", "[dstream]")
{
    autogen_dir dir;

    SECTION("Inexisting directory")
    {
        REQUIRE_THROWS_AS(mp::idstream(NON_EXISTING_DIR), std::system_error);
    }

    SECTION("Read existing directory")
    {
        mp::idstream ds(dir.get_path());
        REQUIRE(ds);

        SECTION("Read single value")
        {
            std::string entry;
            ds >> entry;

            REQUIRE_THAT(dir.get_entries(), Catch::Matchers::VectorContains(entry));
        }

        SECTION("Read all, value by value")
        {
            std::string entry;
            while (ds >> entry)
            {
                REQUIRE_THAT(dir.get_entries(), Catch::Matchers::VectorContains(entry));
            }
        }

        SECTION("Read all into container")
        {
            // NOTE: Order is guaranteed, reading into a set

            std::set<std::string> entries;
            ds >> entries;

            auto expected_entries = dir.get_sorted_entries();
            REQUIRE(std::equal(expected_entries.begin(), expected_entries.end(),
                               entries.begin()));
        }

        SECTION("Read all into insert iterator")
        {
            // NOTE: Order is not guaranteed

            std::vector<std::string> entries;
            ds >> std::inserter(entries, entries.end());;

            auto expected_entries = dir.get_entries();
            REQUIRE(expected_entries.size() == entries.size());
            for (auto& entry : entries)
            {
                REQUIRE_THAT(expected_entries, Catch::Matchers::VectorContains(entry));
            }
        }

        SECTION("Rewind")
        {
            // NOTE: Order is not guaranteed

            std::string entry;
            ds >> entry;
            REQUIRE_THAT(dir.get_entries(), Catch::Matchers::VectorContains(entry));

            ds.rewind();

            // NOTE: Order is not guaranteed, reading into a set

            std::set<std::string> entries;
            ds >> entries;
            auto expected_entries = dir.get_sorted_entries();
            REQUIRE(std::equal(expected_entries.begin(), expected_entries.end(),
                               entries.begin()));
        }
    }
}

