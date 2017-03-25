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

#include <iostream>

#include "Mpopen.hpp"

using namespace std;

void impopenReadByLine()
{
    mp::impopen reader("hostname");

    std::string line;
    while (reader >> line)
    {
        cout << line << endl;
    }
}

void impopenReadAll()
{
    mp::impopen reader("ls -l");

    std::vector<std::string> output;
    reader >> output;

    std::vector<std::string>::const_iterator it;
    for (it = output.begin(); it != output.end(); ++it)
    {
        cout << *it;
    }
}

void ompopen()
{
    mp::ompopen writer("wall");

    writer << "testing" << " " << "modern output popen" << "\n";
}

int main()
{
    impopenReadByLine();
    impopenReadAll();

    ompopen();

    return 0;
}
