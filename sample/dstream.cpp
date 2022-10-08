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

#include <iostream>
#include <set>

#include "modernposix/dstream.hpp"

void read(const std::string& dir)
{
    mp::idstream ds(dir);

    std::set<std::string> output;
    ds >> output;

    for (auto entry : output)
    {
        std::cout << entry << std::endl;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <dir>" << std::endl;
        return 2;
    }
    std::string dir = argv[1];

    read(dir);
    return 0;
}
