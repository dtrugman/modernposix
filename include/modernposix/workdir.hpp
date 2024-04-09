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

#ifndef MP_WORKDIR_HPP
#define MP_WORKDIR_HPP

#include <linux/limits.h>

#include <string.h>
#include <unistd.h>

#include <string>

#include "modernposix.hpp"
#include "last_error.hpp"

namespace mp {
namespace workdir {

inline std::string getcwd()
{
    char buffer[PATH_MAX];
    if (::getcwd(buffer, sizeof(buffer)) == NULL)
    {
        throw last_error::to_exception("Get current working directory failed");
    }

    return std::string(buffer);
}

inline void chdir(const std::string& dir)
{
    if (::chdir(dir.c_str()) != 0)
    {
        throw last_error::to_exception("Change working directory failed");
    }
}

class pushd
{
public:
    pushd(const std::string& dir)
        : _prev(getcwd())
    {
        chdir(dir);
    }

    ~pushd()
    {
        chdir(_prev);
    }

private:
    const std::string _prev;
};

} // namespace workdir
} // namespace mp

#endif // MP_WORKDIR_HPP
