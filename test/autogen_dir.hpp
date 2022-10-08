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

#ifndef MP_TEST_AUTOGEN_DIR_HPP
#define MP_TEST_AUTOGEN_DIR_HPP

#include <set>
#include <vector>

/*
 * Creates a temporary dir and populates it with some files and sub-directories.
 * Deletes everything upon destruction.
 */
class autogen_dir
{
public:
    autogen_dir();
    ~autogen_dir();

    const std::string& get_path();
    const std::vector<std::string> get_entries();
    const std::set<std::string> get_sorted_entries();

private:
    static std::string create_temp_dir();
    static void remove_dir(const std::string& dirname);
    static void create_subdir(const std::string& dirname, const std::string& subdirname);
    static void remove_subdir(const std::string& dirname, const std::string& subdirname);
    static void create_file(const std::string& dirname, const std::string& filename);
    static void remove_file(const std::string& dirname, const std::string& filename);

private:
    static const std::vector<std::string> AUTO_ENTRIES;
    static const std::vector<std::string> FILE_ENTRIES;
    static const std::vector<std::string> SUBDIR_ENTRIES;

private:
    const std::string _dirname;
};

#endif // MP_TEST_AUTOGEN_DIR_HPP
