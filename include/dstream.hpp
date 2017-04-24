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
public: // C'tors & D'tors
    dstream(const std::string & dirname)
        : _fd(NULL)
    {
        _active = open(dirname);
    }

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

    dstream & operator>>(std::string & name)
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

    dstream & operator>>(std::set<std::string> & output)
    {
        std::string name;
        while (*this >> name)
        {
            output.insert(name);
        }

        return *this;
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

protected: // Methods
    inline const char * error()
    {
        return strerror(errno);
    }

private: // Members
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

} // namespace mp

#endif // MP_DIRECTORY_STREAM_HPP