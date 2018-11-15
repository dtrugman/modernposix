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

#include "../include/workdir.hpp"

using namespace std;

int main()
{
    bool success;

    string dir;
    cout << "Enter dir: ";
    cin >> dir;

    cout << "Changing workdir to [" << dir << "]" << endl;
    success = mp::workdir::chdir(dir);
    cout << "Successful? " << std::boolalpha << success << endl;

    string cwd;
    success = mp::workdir::getcwd(cwd);
    cout << "Current workdir = " << cwd << endl;

    return 0;
}
