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

#ifndef MP_PROCESS_STREAM_HPP
#define MP_PROCESS_STREAM_HPP

#include "modernposix.hpp"

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <vector>
#include <string>

namespace mp
{

class pstream // aka process stream
{
public: // D'tors
    virtual ~pstream()
    {
        try
        {
            close();
        }
        catch (std::runtime_error & ex)
        {
            // Prevents exception from leaving destructor
        }
    }

public: // Operators
    inline operator bool() const
    {
        return _active;
    }

public: // Methods
    int close()
    {
        if (NULL == _fd)
        {
            // We are already closed
            return 0;
        }

        int exitcode = pclose(_fd);
        if (-1 == exitcode)
        {
            MP_RETURN_OR_THROW_EX(exitcode, std::runtime_error, error());
        }

        _fd = NULL;
        _active = false;
        
        return exitcode;
    }

protected: // C'tors
    pstream(const std::string & command, const std::string & type)
        : _fd(NULL)
    {
        _active = open(command, type);
    }

protected: // Methods
    inline std::string error() const
    {
        return std::string(strerror(errno));
    }

protected: // Members
    FILE * _fd;
    bool   _active;

private: // Methods
    inline bool open(const std::string & command, const std::string & type)
    {
        _fd = popen(command.c_str(), type.c_str());
        if (NULL == _fd)
        {
            MP_RETURN_OR_THROW_EX(false, std::runtime_error, error());
        }

        return true;
    }
};

class ipstream : public pstream
{
public: // C'tors
    ipstream(const std::string & command, size_t bufferSize = DEFAULT_BUFFER_SIZE)
        : pstream(command, "r"), _bufferSize(bufferSize)
    {
        // Do nothing
    }

public: // Operators
    ipstream & operator>>(std::string & line)
    {
        if (_active)
        {
            char buffer[_bufferSize];
            _active = (fgets(buffer, sizeof(buffer), _fd) != NULL);
            if (_active)
            {
                line.assign(buffer);
            }
        }
        
        return *this;
    }

    ipstream & operator>>(std::vector<std::string> & output)
    {
        std::string line;
        while (*this >> line)
        {
            output.push_back(line);
        }

        return *this;
    }

private: // Consts
    static const size_t DEFAULT_BUFFER_SIZE = 1024;

private: // Members
    const size_t _bufferSize;
};

class opstream : public pstream
{
public: // C'tors
    opstream(const std::string & command)
        : pstream(command, "w")
    {
        // Do nothing
    }

public: // Operators
    opstream & operator<<(const char * buffer)
    {
        if (_active)
        {
            _active = (fputs(buffer, _fd) != EOF);
        }

        return *this;
    }

    opstream & operator<<(std::string & buffer)
    {
        return *this << buffer.c_str();
    }
};

} // namespace mp

#endif // MP_PROCESS_STREAM_HPP