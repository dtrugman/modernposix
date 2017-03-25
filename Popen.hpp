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

class Popen
{
public: // Types
    enum Type
    {
        READ,
        WRITE
    };

public: // C'tors & D'tors
    Popen(const std::string & command, Type type)
        : _fd(NULL), _type(type)
    {
        if (!validate(command, type)) return;
        if (!open(command, type) return;
    }

    virtual ~Popen()
    {
        if (NULL != _fd) (void)pclose(_fd);
    }

public: // Operators
    operator bool() const
    {
        return (NULL != fd);
    }

private:
    inline bool validate(const std::string & command, Type type)
    {
        switch(type)
        {
            case READ:
            case WRITE:
                // Everything is OK
                break;
            
            default:
                RETURN_OR_THROW_EX(false, std::runtime_error, "Bad type specified");
        }

        return true;
    }

    inline bool open(const std::string & command, Type type)
    {
        const char * c = command.c_str();
        const char * t = (type == READ) ? "r" : "w";

        _fd = popen(c, t);
        if (NULL == _fd)
        {
            RETURN_OR_THROW_EX(false, std::runtime_error, "Underlying popen failed, fork(), pipe() or memory allocation error");
        }

        return true;
    }

private:
    FILE * _fd;
    Type   _type;
};

} // namespace mopo

#endif // MODERN_POPEN_HPP