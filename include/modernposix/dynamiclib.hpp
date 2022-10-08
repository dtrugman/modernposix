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

#ifndef MP_DYNAMICLIB_HPP
#define MP_DYNAMICLIB_HPP

#include <dlfcn.h>

#include <string>

#include "modernposix.hpp"
#include "dl_last_error.hpp"

namespace mp {

class dynamiclib
{
public: // C'tors & D'tors
    dynamiclib(const std::string& file, int mode = DEFAULT_MODE)
        : _handle(open(file, mode)) {}

    virtual ~dynamiclib()
    {
        CALL_AND_SUPPRESS_EXCEPTIONS(close);
    }

public: // Operators
    operator bool() const
    {
        return (_handle != NULL);
    }

public: // Methods
    void close()
    {
        if (_handle == NULL)
        {
            return;
        }

        int err = dlclose(_handle);
        if (err)
        {
            throw dl_last_error::to_exception("Close library failed");
        }

        _handle = NULL;
    }

    void* symbol(const std::string& name)
    {
        if (_handle == NULL)
        {
            throw std::runtime_error("Load symbol failed: Library not loaded");
        }

        dl_last_error::clear();

        void* addr = dlsym(_handle, name.c_str());
        if (addr != NULL)
        {
            return addr;
        }

        char* error = dl_last_error::get();
        if (error == NULL)
        { // The value of the symbol is actually NULL
            return addr;
        }

        throw dl_last_error::to_exception("Load symbol failed", error);
    }

private: // Methods
    static void* open(const std::string& file, int mode)
    {
        void* handle = dlopen(file.c_str(), mode);
        if (handle == NULL)
        {
            throw dl_last_error::to_exception("Open library failed");
        }
        return handle;
    }

private: // Consts
    static const int DEFAULT_MODE = RTLD_LAZY | RTLD_LOCAL;

private: // Members
    void* _handle;
};

} // namespace mp

#endif // MP_DYNAMICLIB_HPP
