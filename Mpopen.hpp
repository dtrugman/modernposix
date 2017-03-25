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

#include <string>

namespace mp
{

class mpopen
{
public: // D'tors
    virtual ~mpopen()
    {
        close();        
    }

public: // Operators
    operator bool() const
    {
        return _active;
    }

public: // Methods
    void close()
    {
        _active = false;

        if (NULL != _fp) (void)pclose(_fp);
    }

protected: // C'tors
    mpopen(const std::string & command, const std::string & type)
        : _fp(NULL)
    {
        _active = open(command, type);
    }

protected: // Members
    FILE * _fp;
    bool   _active;

private: // Methods
    inline bool open(const std::string & command, const std::string & type)
    {
        _fp = popen(command.c_str(), type.c_str());
        if (NULL == _fp)
        {
            RETURN_OR_THROW_EX(false, std::runtime_error, "Underlying popen failed, fork(), pipe() or memory allocation error");
        }

        return true;
    }
};

class impopen : public mpopen
{
public: // C'tors
    impopen(const std::string & command, size_t bufferSize = DEFAULT_BUFFER_SIZE)
        : mpopen(command, "r"), _bufferSize(bufferSize)
    {
        // Do nothing
    }

public: // Operators
    impopen & operator>>(std::string & line)
    {
        if (_active)
        {
            char buffer[_bufferSize];
            _active = (fgets(buffer, sizeof(buffer), _fp) != NULL);
            if (_active)
            {
                line.assign(buffer);
            }
        }
        
        return *this;
    }

private: // Consts
    static const size_t DEFAULT_BUFFER_SIZE = 1024;

private: // Members
    const size_t _bufferSize;
};

class ompopen : public mpopen
{
public: // C'tors
    ompopen(const std::string & command)
        : mpopen(command, "w")
    {
        // Do nothing
    }

public: // Operators
    ompopen & operator<<(const char * buffer)
    {
        if (_active)
        {
            _active = (fputs(buffer, _fp) != EOF);
        }

        return *this;
    }

    ompopen & operator<<(std::string & buffer)
    {
        return *this << buffer.c_str();
    }
};

} // namespace mp

#endif // MODERN_POPEN_HPP