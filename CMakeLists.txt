#
# Copyright (C) 2019 Ole-André Rodlie
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#

cmake_minimum_required(VERSION 3.0)
project(stackwalker VERSION 2019)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(BREAKPAD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/breakpad)
set(BREAKPAD_BIN ${CMAKE_BINARY_DIR}/breakpad)
set(STACKWALKER ${CMAKE_CURRENT_SOURCE_DIR}/minidump-stackwalk)
set(BREAKDOWN ${CMAKE_CURRENT_SOURCE_DIR}/breakdown)
set(BREAKDOWN_URL "https://stackwalker.000webhostapp.com/reports")

file(MAKE_DIRECTORY ${BREAKPAD_BIN})

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Git QUIET)

pkg_search_module(CURL REQUIRED libcurl)
pkg_search_module(ZIP REQUIRED libzip)
pkg_search_module(JSON REQUIRED jsoncpp)

option(MXE32_STATIC "Build using MXE i686 static" OFF)
option(MXE64_STATIC "Build using MXE x86_64 static" OFF)
option(MXE32_SHARED "Build using MXE i686 shared" OFF)
option(MXE64_SHARED "Build using MXE x86_64 shared" OFF)

if(NOT EXISTS ${BREAKPAD_SRC}/configure)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

if(NOT EXISTS ${BREAKPAD_BIN}/src/libbreakpad.a)
    message("-- MISSING BREAKPAD (will try to build)")
    if(MXE32_STATIC)
        set(BREAKPAD_OPTIONS "--host=i686-w64-mingw32.static")
    elseif(MXE64_STATIC)
        set(BREAKPAD_OPTIONS "--host=x86_64-w64-mingw32.static")
    elseif(MXE32_SHARED)
        set(BREAKPAD_OPTIONS "--host=i686-w64-mingw32.shared")
    elseif(MXE64_SHARED)
        set(BREAKPAD_OPTIONS "--host=x86_64-w64-mingw32.shared")
    endif()
    execute_process(
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/build_breakpad.sh ${BREAKPAD_SRC} ${BREAKPAD_OPTIONS}
        WORKING_DIRECTORY ${BREAKPAD_BIN}
    )
endif()

if(CMAKE_BUILD_TYPE MATCHES "^(release|Release|RELEASE)$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
else()
    message("-- DEBUG BUILD")
endif()
add_compile_options(-Wall -Wextra ${CURL_CFLAGS})
add_definitions(-D__STDC_FORMAT_MACROS=1 -Wno-format)
add_definitions(-DBREAKDOWN_URL=${BREAKDOWN_URL})
add_definitions(-DBREAKDOWN_VERSION=${PROJECT_VERSION})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${BREAKDOWN}
    ${BREAKPAD_SRC}/src
    ${STACKWALKER}
    ${CURL_INCLUDE_DIRS}
    ${ZIP_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)

add_executable(
    breakdown
    ${STACKWALKER}/http_symbol_supplier.cc
    ${BREAKDOWN}/main.cpp
    ${BREAKDOWN}/breakdown.cpp
    ${BREAKDOWN}/breakdown.ui
)
add_executable(
    stackwalker
    ${STACKWALKER}/http_symbol_supplier.cc
    ${STACKWALKER}/stackwalker.cc
)

target_link_libraries(
    breakdown
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    ${BREAKPAD_BIN}/src/libbreakpad.a
    ${BREAKPAD_BIN}/src/third_party/libdisasm/libdisasm.a
    ${CURL_STATIC_LIBRARIES}
    ${ZIP_STATIC_LIBRARIES}
    ${JSON_STATIC_LIBRARIES}
)

target_link_libraries(
    stackwalker
    ${BREAKPAD_BIN}/src/libbreakpad.a
    ${BREAKPAD_BIN}/src/third_party/libdisasm/libdisasm.a
    ${CURL_STATIC_LIBRARIES}
    ${ZIP_STATIC_LIBRARIES}
    ${JSON_STATIC_LIBRARIES}
)
