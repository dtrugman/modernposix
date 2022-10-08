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

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "modernposix/last_error.hpp"

#include "autogen_dir.hpp"

const std::vector<std::string> autogen_dir::AUTO_ENTRIES = {
    ".", ".."
};
const std::vector<std::string> autogen_dir::FILE_ENTRIES = {
    ".hiddenfile", "sourcefile.c", "noext", "testfile.txt"
};
const std::vector<std::string> autogen_dir::SUBDIR_ENTRIES = {
    "__a", ".b"
};

autogen_dir::autogen_dir() : _dirname(create_temp_dir())
{
    for (const auto& entry : FILE_ENTRIES)
    {
        create_file(_dirname, entry);
    }

    for (const auto& entry : SUBDIR_ENTRIES)
    {
        create_subdir(_dirname, entry);
    }
}

autogen_dir::~autogen_dir()
{
    for (const auto& entry : SUBDIR_ENTRIES)
    {
        remove_subdir(_dirname, entry);
    }

    for (const auto& entry : FILE_ENTRIES)
    {
        remove_file(_dirname, entry);
    }

    remove_dir(_dirname);
}

const std::string& autogen_dir::get_path()
{
    return _dirname;
}

const std::vector<std::string> autogen_dir::get_entries()
{
    std::vector<std::string> all_entries;
    all_entries.insert(all_entries.end(), AUTO_ENTRIES.begin(), AUTO_ENTRIES.end());
    all_entries.insert(all_entries.end(), FILE_ENTRIES.begin(), FILE_ENTRIES.end());
    all_entries.insert(all_entries.end(), SUBDIR_ENTRIES.begin(), SUBDIR_ENTRIES.end());
    return all_entries;
}

const std::set<std::string> autogen_dir::get_sorted_entries()
{
    std::vector<std::string> all_entries = get_entries();
    std::set<std::string> sorted_entries(all_entries.begin(), all_entries.end());
    return sorted_entries;
}

std::string autogen_dir::create_temp_dir()
{
    char dirname[] = "mp_test_XXXXXX";
    char* rv = mkdtemp(dirname);
    if (rv == NULL)
    {
        throw mp::last_error::to_exception("Create temp directory failed");
    }

    return std::string(dirname);
}

void autogen_dir::remove_dir(const std::string& dirname)
{
    int err = rmdir(dirname.c_str());
    if (err)
    {
        throw mp::last_error::to_exception("Remove directory failed");
    }
}

void autogen_dir::create_subdir(const std::string& dirname, const std::string& subdirname)
{
    std::string path = dirname + "/" + subdirname;
    int err = mkdir(path.c_str(), S_IRUSR | S_IRGRP | S_IROTH);
    if (err)
    {
        throw mp::last_error::to_exception("Create directory failed");
    }
}

void autogen_dir::remove_subdir(const std::string& dirname, const std::string& subdirname)
{
    std::string path = dirname + "/" + subdirname;
    remove_dir(path);
}

void autogen_dir::create_file(const std::string& dirname, const std::string& filename)
{
    std::string path = dirname + "/" + filename;
    int fd = open(path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd == -1)
    {
        throw mp::last_error::to_exception("Open file failed");
    }
}

void autogen_dir::remove_file(const std::string& dirname, const std::string& filename)
{
    std::string path = dirname + "/" + filename;
    int err = unlink(path.c_str());
    if (err)
    {
        throw mp::last_error::to_exception("Remove file failed");
    }
}
