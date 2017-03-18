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

#ifndef MODERN_CONFIG_HPP
#define MODERN_CONFIG_HPP

#include <stdexcept>

namespace mopo
{

/**
 * When defined, instead of throwing exceptions,
 * the library handles errors by returning NULL values
 * or failing subsequent operations
 */
//#define NO_THROW

#ifdef NO_THROW
    #define RETURN_OR_THROW_EX(retval, type, ...) return retval
#else
    #define RETURN_OR_THROW_EX(retval, type, ...) throw type(__VA_ARGS__)
#endif

} // namespace mopo

#endif // MODERN_CONFIG_HPP