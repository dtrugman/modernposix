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
#include "utils.hpp"

#include "../include/dstream.hpp"

#include <dirent.h>

using namespace std;

static const char * NON_EXISTING_DIR = "nonexistingdir";

static const char * TEST_DIR = "utils/testdir";
static const char * TEST_DIR_ENTRIES[] = { ".", "..", ".hiddenfile", "sourcefile.c", "subdir", "textfile.txt" }; // Sorted in alphabetical order

int findEntry(const string & entry)
{
    int index = -1;
    for (size_t i = 0; -1 == index && i < LENGTHOF(TEST_DIR_ENTRIES); ++i)
    {
        if (entry.compare(TEST_DIR_ENTRIES[i]) == 0)
        {
            index = i;
        }
    }

    return index;
}

bool hasEntry(const string & entry)
{
    return findEntry(entry) != -1;
}

bool hasAllEntriesSorted(const set<string> & entries)
{
    if (entries.size() != LENGTHOF(TEST_DIR_ENTRIES))
    {
        return false;
    }

    bool hasAll = true;

    set<string>::const_iterator it = entries.begin();
    for (size_t i = 0; i < LENGTHOF(TEST_DIR_ENTRIES); ++i)
    {
        hasAll &= (it->compare(TEST_DIR_ENTRIES[i]) == 0);
        ++it;
    }

    return hasAll;
}

TEST_CASE("Directory stream", "[dstream]")
{
    SECTION("Unexisting directory")
    {
        REQUIRE_INIT_FAILURE(mp::dstream, std::runtime_error, NON_EXISTING_DIR);
    }

    SECTION("Read existing directory")
    {
        REQUIRE_INIT_SUCCESS(mp::dstream, dir, TEST_DIR);

        SECTION("Read single value")
        {
            // Order is not guarenteed, so we just make sure it's one of the entries

            string entry;
            dir >> entry;
            REQUIRE(hasEntry(entry));
        }

        SECTION("Read all, value by value")
        {
            // Order is not guarenteed, so we just make sure it's one of the entries

            bool found[LENGTHOF(TEST_DIR_ENTRIES)] = { 0 };

            string entry;
            while(dir >> entry)
            {
                int index = findEntry(entry);
                CHECK(index != -1);
                if (index != -1)
                {
                    CHECK(!found[index]);
                    found[index] = true;
                }
            }
        }

        SECTION("Read all at once")
        {
            // Order is guarenteed, as we use a set

            set<string> entries;
            dir >> entries;
            REQUIRE(hasAllEntriesSorted(entries));
        }

        SECTION("Rewind")
        {
            // Order is not guarenteed, so we just make sure it's one of the entries

            string entry;
            dir >> entry;
            REQUIRE(hasEntry(entry));

            dir.rewind();

            // Order is guarenteed, as we use a set

            set<string> entries;
            dir >> entries;
            REQUIRE(hasAllEntriesSorted(entries));
        }
    }
}

