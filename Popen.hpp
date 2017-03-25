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

#ifndef MODERN_POPEN_HPP
#define MODERN_POPEN_HPP

#include "Config.hpp"

#include <stdio.h>

namespace mopo
{

class mpopen
{
public: // D'tors
    virtual ~mpopen()
    {
        if (NULL != _fd) (void)pclose(_fd);
    }

public: // Operators
    operator bool() const
    {
        return (NULL != _fd);
    }

protected: // C'tors
    mpopen(const std::string & command, const std::string & type)
        : _fd(NULL)
    {
        open(command, type);
    }

private: // Methods
    inline bool open(const std::string & command, const std::string & type)
    {
        _fd = popen(command.c_str(), type.c_str());
        if (NULL == _fd)
        {
            RETURN_OR_THROW_EX(false, std::runtime_error, "Underlying popen failed, fork(), pipe() or memory allocation error");
        }

        return true;
    }

private: // Members
    FILE * _fd;
};

class impopen : public mpopen
{
public: // C'tors
    impopen(const std::string & command)
        : mpopen(command, "r")
    {
        // Do nothing
    }
};

class ompopen : public mpopen
{
public: // C'tors
    ompopen(const std::string & command)
        : mpopen(command, "w")
    {
        // Do nothing
    }
};

} // namespace mopo

#endif // MODERN_POPEN_HPP