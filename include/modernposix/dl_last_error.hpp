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

#ifndef MP_DL_LAST_ERROR_HPP
#define MP_DL_LAST_ERROR_HPP

#include <errno.h>

#include <string>
#include <sstream>

namespace mp {
namespace dl_last_error {

inline char* get()
{
    return dlerror();
}

inline void clear()
{
    // From man dlerror(3):
    // It returns NULL if no errors have occurred since initialization
    // or since it was last called
    dlerror();
}

inline std::string to_string()
{
    static const std::string NO_ERR_OR_CLEARED("Either there was no error or it was cleared");
    const char* error = get();
    return error ? std::string(error) : NO_ERR_OR_CLEARED;
}

inline std::runtime_error to_exception(const char* error = get())
{
    return std::runtime_error(error);
}

inline std::runtime_error to_exception(const std::string& message, const char *error = get())
{
    std::ostringstream what;
    what << message << ": " << error;
    return std::runtime_error(what.str());
}

} // namespace dl_last_error
} // namespace mp

#endif // MP_DL_LAST_ERROR_HPP
