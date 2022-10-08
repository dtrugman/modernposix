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

#ifndef MP_PSTREAM_HPP
#define MP_PSTREAM_HPP

#include <cstdio>
#include <cstring>

#include <vector>
#include <string>

#include "modernposix.hpp"
#include "last_error.hpp"

namespace mp {

class pstream
{
public:
    virtual ~pstream()
    {
        CALL_AND_SUPPRESS_EXCEPTIONS(close);
    }

public:
    operator bool() const
    {
        return _active;
    }

public:
    int close()
    {
        if (_fd == NULL)
        {
            return 0; // Already closed
        }

        int exitcode = pclose(_fd);
        if (exitcode == -1)
        {
            throw last_error::to_exception("Close sub-process failed");
        }

        _fd = NULL;
        _active = false;

        return exitcode;
    }

protected:
    static const bool DEFAULT_CLOSE_ON_EXEC = false;

    enum class direction
    {
        read,
        write
    };

protected:
    pstream(const std::string& command, direction dir, bool close_on_exec)
        : _fd(open(command, type(dir, close_on_exec))), _active(true) {}

protected:
    FILE* _fd;
    bool  _active;

private:
    static std::string type(direction dir, bool close_on_exec)
    {
        std::string type = (dir == direction::read) ? "r" : "w";
        if (close_on_exec)
        {
            type += "e";
        }
        return type;
    }

    static FILE* open(const std::string& command, const std::string& type)
    {
        FILE* fd = popen(command.c_str(), type.c_str());
        if (fd == NULL)
        {
            throw last_error::to_exception("Open sub-process failed");
        }
        return fd;
    }
};

// Input Process Stream
// Create a subprocess and read its output.
class ipstream : public pstream
{
public:
    ipstream(const std::string& command,
             bool close_on_exec = DEFAULT_CLOSE_ON_EXEC,
             size_t buffer_size = DEFAULT_BUFFER_SIZE) :
        pstream(command, direction::read, close_on_exec), _buffer_size(buffer_size) {}

public:
    ipstream& operator>>(std::string& output)
    {
        if (!_active)
        {
            return *this;
        }

        std::vector<char> buffer(_buffer_size);
        char *rv = fgets(buffer.data(), buffer.size(), _fd);
        if (rv == buffer.data())
        {
            output.assign(buffer.data());
            return *this;
        }

        if (!ferror(_fd))
        {
            _active = false;
            return *this;
        }

        throw last_error::to_exception("Read from sub-process failed");
    }

    template <class InsertIter>
    ipstream& operator>>(InsertIter output)
    {
        std::string entry_name;
        while (*this >> entry_name)
        {
            *output = entry_name;
            ++output;
        }

        return *this;
    }

    template <template<typename...> class Container>
    ipstream& operator>>(Container<std::string>& output)
    {
        return *this >> std::inserter(output, output.end());
    }

private:
    static const size_t DEFAULT_BUFFER_SIZE = 1024;

private:
    const size_t _buffer_size;
};

class opstream : public pstream
{
public:
    opstream(const std::string& command, bool close_on_exec = false) :
        pstream(command, direction::write, close_on_exec) {}

public:
    opstream& operator<<(const char* output)
    {
        if (!_active)
        {
            return *this;
        }

        int rv = fputs(output, _fd);
        if (rv != EOF)
        {
            return *this;
        }

        if (!ferror(_fd))
        {
            _active = false;
            return *this;
        }

        throw last_error::to_exception("Write to sub-process failed");
    }

    opstream& operator<<(const std::string& output)
    {
        return *this << output.c_str();
    }

    template <template<typename> class Container>
    opstream& operator<<(const Container<std::string>& output)
    {
        for (const std::string& line : output)
        {
            *this << output;
        }

        return *this;
    }
};

} // namespace mp

#endif // MP_PSTREAM_HPP
