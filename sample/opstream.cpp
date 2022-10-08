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
#include <iterator>
#include <vector>
#include <string>

#include "modernposix/pstream.hpp"

int main(int argc, char **argv)
{
    // HINT: Run it with: ./out/opstream "grep 'cool' -" cold cooler coolio coooooooombo box

    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <command> <output>..." << std::endl;
        return 2;
    }
    std::string command = argv[1];
    mp::opstream writer(command);

    for (int i = 2; i < argc; ++i)
    {
        writer << argv[i] << "\n";
    }

    return 0;
}
