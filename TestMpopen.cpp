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

TEST_CASE("Input modern popen", "[popen]")
{
    char hostname[1024];
    hostname[sizeof(hostname) - 1] = '\0';
    gethostname(hostname, sizeof(hostname) - 1);

    SECTION("Get hostname")
    {
        mp::impopen reader("hostname");
        
        std::string line;
        reader >> line;
        line.erase(line.size() - 1);

        REQUIRE(line.compare(hostname) == 0);
    }
}
