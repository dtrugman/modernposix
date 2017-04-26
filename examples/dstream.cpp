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

#include <iostream>

#include "../include/dstream.hpp"

using namespace std;

void idstream_readline(const string & dir)
{
    mp::idstream ds(dir);

    string entry;
    while (ds >> entry)
    {
        cout << entry << endl;
    }
}

void idstream_readall(const string & dir)
{

    mp::idstream ds(dir);

    set<string> output;
    ds >> output;

    set<string>::const_iterator it;
    for (it = output.begin(); it != output.end(); ++it)
    {
        cout << *it << endl;
    }
}

int main()
{
    string dir;
    cout << "Enter dir: ";
    cin >> dir;

    cout << "Reading line by line:" << endl;
    idstream_readline(dir);
    cout << endl << "Reading all at once:" << endl;
    idstream_readall(dir);

    return 0;
}
