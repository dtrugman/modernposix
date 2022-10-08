cmake_minimum_required (VERSION 3.5)

project (mp)

# ------------------------------------------------------------------------
# Flags that are expected to be shared when added as sub-directory
# ------------------------------------------------------------------------

if (NOT CMAKE_BUILD_TYPE)
    message (STATUS "Build type not specified. Set by default to DEBUG!")
    set (CMAKE_BUILD_TYPE Debug)
endif ()

# ------------------------------------------------------------------------
# Flags that are NOT expected to be shared when added as sub-directory
# ------------------------------------------------------------------------

option (mp_BUILD_ASAN "Enable address sanitizer" OFF)
option (mp_BUILD_SAMPLES "Build samples" ON)
option (mp_BUILD_TESTS "Build tests" ON)

# ------------------------------------------------------------------------
# Actual configuration
# ------------------------------------------------------------------------

add_compile_options ("$<$<COMPILE_LANGUAGE:CXX>:-std=c++11>")
add_compile_options (-Wall -Wextra -pedantic -Werror)

include_directories (include)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if (mp_BUILD_SAMPLES OR mp_BUILD_TESTS)
    set (mp_UTILS_ROOT "utils")
    add_library (test_c SHARED "${mp_UTILS_ROOT}/lib/c/lib.c")
    add_library (test_cpp SHARED "${mp_UTILS_ROOT}/lib/cpp/lib.cpp")
endif ()

if (mp_BUILD_SAMPLES)
    set (mp_SAMPLES_ROOT "sample")
    add_executable (dynamiclib "${mp_SAMPLES_ROOT}/dynamiclib.cpp")
    target_link_libraries (dynamiclib PRIVATE dl)
    target_include_directories (dynamiclib PRIVATE "${mp_UTILS_ROOT}")

    add_executable (ipstream "${mp_SAMPLES_ROOT}/ipstream.cpp")
    add_executable (opstream "${mp_SAMPLES_ROOT}/opstream.cpp")
    add_executable (dstream "${mp_SAMPLES_ROOT}/dstream.cpp")
    add_executable (workdir "${mp_SAMPLES_ROOT}/workdir.cpp")
endif ()

if (mp_BUILD_TESTS)
    set (mp_TESTS_ROOT "test")
    aux_source_directory (${mp_TESTS_ROOT} mp_TEST_SOURCES)
    add_executable (unittest ${mp_TEST_SOURCES})
    target_link_libraries (unittest PRIVATE dl)
    target_include_directories (unittest PRIVATE "${mp_UTILS_ROOT}")
endif()
