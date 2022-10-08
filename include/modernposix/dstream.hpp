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

#ifndef MP_DSTREAM_HPP
#define MP_DSTREAM_HPP

#include <dirent.h>

#include <string>

#include "modernposix.hpp"
#include "last_error.hpp"

namespace mp {

// (Base) Directory Stream
class dstream
{
public: // D'tors
    virtual ~dstream()
    {
        CALL_AND_SUPPRESS_EXCEPTIONS(close);
    }

public: // Operators
    operator bool() const
    {
        return _active;
    }

public: // Methods
    void close()
    {
        if (_fd == NULL)
        {
            return; // Already closed
        }

        int err = closedir(_fd);
        if (err)
        {
            throw last_error::to_exception("Close directory failed");
        }

        _fd = NULL;
        _active = false;
    }

    void rewind()
    {
        rewinddir(_fd);
    }

protected: // C'tors
    dstream(const std::string& dirname)
        : _fd(open(dirname)), _active(true) {}

protected: // Members
    DIR* _fd;
    bool _active;

private: // Methods
    static DIR* open(const std::string& dirname)
    {
        DIR* fd = opendir(dirname.c_str());
        if (fd == NULL)
        {
            throw last_error::to_exception("Open directory failed");
        }
        return fd;
    }
};

// Input Directory Stream
// Read all entries in a directory.
// Note: The specific order depends on the filesystem implementation.
// It is unlikely that the names will be sorted in any fashion.
// See man readdir(3) for more information.
class idstream : public dstream
{
public: // C'tors
    idstream(const std::string& dirname)
        : dstream(dirname) {}

public: // Operators
    idstream& operator>>(std::string& entry_name)
    {
        if (!_active)
        {
            return *this;
        }

        last_error::clear();

        const struct dirent* entry = readdir(_fd);
        if (entry != NULL)
        {
            entry_name = entry->d_name;
            return *this;
        }

        if (!last_error::is_set())
        {
            _active = false;
            return *this;
        }

        throw last_error::to_exception("Read entry failed");
    }

    template <class InsertIter>
    idstream& operator>>(InsertIter output)
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
    idstream& operator>>(Container<std::string>& output)
    {
        return *this >> std::inserter(output, output.end());
    }
};

// No Output Directory Stream in the POSIX API

} // namespace mp

#endif // MP_DSTREAM_HPP
