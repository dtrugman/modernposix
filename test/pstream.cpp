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

#include "modernposix/pstream.hpp"

#include "catch.hpp"
#include "autogen_dir.hpp"

std::string get_hostname()
{
    char buffer[1024];
    buffer[sizeof(buffer) - 1] = '\0';
    gethostname(buffer, sizeof(buffer) - 1);

    return std::string(buffer);
}

TEST_CASE("Process stream", "[pstream]")
{
    std::string hostname = get_hostname();

    SECTION("Input")
    {
        SECTION("Single line")
        {
            mp::ipstream reader("hostname");
            std::string line;
            reader >> line;
            line.pop_back();

            REQUIRE(line == hostname);
        }

        SECTION("Multi line")
        {
            autogen_dir dir;

            std::string command("ls -a1 ");
            command += dir.get_path();

            mp::ipstream reader(command);
            std::vector<std::string> output;
            reader >> output;
            for (auto& line : output)
            {
                line.pop_back();
            }

            REQUIRE_THAT(dir.get_entries(), Catch::Matchers::UnorderedEquals(output));
        }
    }

    SECTION("Output & Input")
    {
        const std::string line1 = "modern posix\n";
        const std::string line2 = "is freaking awesome!\n";

        {
            mp::opstream writer("cat > test.txt");
            writer << line1 << line2;
        }

        mp::ipstream reader("cat test.txt; rm test.txt");
        std::vector<std::string> output;
        reader >> output;

        REQUIRE(output[0] == line1);
        REQUIRE(output[1] == line2);
    }
}

