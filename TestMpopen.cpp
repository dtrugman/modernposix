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

#include "Mpopen.hpp"

#include <unistd.h>

using namespace std;

TEST_CASE("Modern popen", "[popen]")
{
    char hostname[1024];
    hostname[sizeof(hostname) - 1] = '\0';
    gethostname(hostname, sizeof(hostname) - 1);

    SECTION("Input")
    {
        SECTION("Single line")
        {
            mp::impopen reader("hostname");
            
            std::string line;
            reader >> line;
            line.erase(line.size() - 1);

            REQUIRE(line.compare(hostname) == 0);
        }

        SECTION("Multi line")
        {
            mp::impopen reader("ls -1");
            
            std::vector<std::string> output;
            reader >> output;
            
            bool foundTestMain, foundTestMpopen;
            foundTestMain = foundTestMpopen = false;

            std::vector<std::string>::const_iterator it;
            for (it = output.begin(); it != output.end(); ++it)
            {
                if (it->compare("TestMain.cpp\n") == 0)
                {
                    foundTestMain = true;
                }

                if (it->compare("TestMpopen.cpp\n") == 0)
                {
                    foundTestMpopen = true;
                }
            }

            REQUIRE(foundTestMain);
            REQUIRE(foundTestMpopen);
        }
    }

    SECTION("Input & Output")
    {
        SECTION("Single line")
        {
            {
                mp::ompopen writer("cat > test.txt");
                writer << "modern posix";
            }

            mp::impopen reader("cat test.txt; rm test.txt");
            
            std::string line;
            reader >> line;

            REQUIRE(line.compare("modern posix") == 0);
        }
    }
}

