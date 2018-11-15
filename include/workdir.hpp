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

#ifndef MP_WORKDIR_HPP
#define MP_WORKDIR_HPP

#include "modernposix.hpp"

#include <linux/limits.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <string>

namespace mp
{

class workdir
{
public:
    static bool getcwd(std::string & dir)
    {
        char buffer[PATH_MAX];
        if (::getcwd(buffer, sizeof(buffer)) == NULL)
        {
            MP_RETURN_OR_THROW_EX(false, std::runtime_error,
                                  "Current working directory path longer than PATH_MAX");
        }
        
        dir.assign(buffer);
        return true;
    }

    static bool chdir(const std::string & dir)
    {
        if (::chdir(dir.c_str()) != 0)
        {
            MP_RETURN_OR_THROW_EX(false, std::runtime_error,
                                  "Change directory failed");
        }

        return true;
    }

    static std::string error()
    {
        return std::string(strerror(errno));
    }
};

} // namespace mp

#endif // MP_WORKDIR_HPP