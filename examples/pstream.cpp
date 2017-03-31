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

#include "../include/pstream.hpp"

using namespace std;

void ipstream_readline()
{
    mp::ipstream reader("hostname");

    string line;
    while (reader >> line)
    {
        cout << line << endl;
    }
}

void ipstream_readall()
{
    mp::ipstream reader("ls -l");

    vector<string> output;
    reader >> output;

    vector<string>::const_iterator it;
    for (it = output.begin(); it != output.end(); ++it)
    {
        cout << *it;
    }
}

void opstream_write()
{
    mp::opstream writer("wall");

    writer << "testing" << " " << "modern output popen" << "\n";
}

int main()
{
    ipstream_readline();
    ipstream_readall();

    opstream_write();

    return 0;
}
