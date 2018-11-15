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

#ifndef MP_DIRECTORY_STREAM_HPP
#define MP_DIRECTORY_STREAM_HPP

#include "modernposix.hpp"

#include <dirent.h>

#include <cstring>
#include <cerrno>

#include <set>
#include <string>

namespace mp
{

class dstream // aka directory stream
{
public: // D'tors
    virtual ~dstream()
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

        int exitcode = closedir(_fd);
        if (0 != exitcode)
        {
            MP_RETURN_OR_THROW_EX(exitcode, std::runtime_error, error());
        }

        _fd = NULL;
        _active = false;
        
        return exitcode;
    }

    void rewind()
    {
        rewinddir(_fd);
    }

protected: // C'tors
    dstream(const std::string & dirname)
        : _fd(NULL)
    {
        _active = open(dirname);
    }

protected: // Methods
    inline std::string error() const
    {
        return std::string(strerror(errno));
    }

protected: // Members
    DIR * _fd;
    bool  _active;

private: // Methods
    inline bool open(const std::string & dirname)
    {
        _fd = opendir(dirname.c_str());
        if (NULL == _fd)
        {
            MP_RETURN_OR_THROW_EX(false, std::runtime_error, error());
        }

        return true;
    }
};

class idstream : public dstream
{
public: // C'tors
    idstream(const std::string & dirname)
        : dstream(dirname)
    {
        // Do nothing
    }

public: // Operators
    // Read line by line
    // Order is not guaranteed
    idstream & operator>>(std::string & name)
    {
        if (_active)
        {
            const struct dirent * de = readdir(_fd);
            _active = (de != NULL);
            if (_active)
            {
                name = de->d_name;
            }
        }
        
        return *this;
    }

    // Read all at once
    // Order is guaranteed to be alphabetical
    idstream & operator>>(std::set<std::string> & output)
    {
        std::string name;
        while (*this >> name)
        {
            output.insert(name);
        }

        return *this;
    }
};

// No odstream because there is no such thing in the POSIX API

} // namespace mp

#endif // MP_DIRECTORY_STREAM_HPP