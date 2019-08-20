#
# Stackwalker <https://github.com/NatronGitHub/stackwalker>
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

file(MAKE_DIRECTORY ${BREAKPAD_BIN})

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Git QUIET)

pkg_search_module(CURL REQUIRED libcurl)
pkg_search_module(ZIP REQUIRED libzip)
pkg_search_module(JSON REQUIRED jsoncpp)

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
    execute_process(
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/breakpad.sh ${BREAKPAD_SRC}
        WORKING_DIRECTORY ${BREAKPAD_BIN}
    )
endif()

if(CMAKE_BUILD_TYPE MATCHES "^(release|Release|RELEASE)$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
else()
    message("-- DEBUG BUILD")
endif()
add_compile_options(-Wall -Wextra)
add_definitions(-D__STDC_FORMAT_MACROS=1 -Wno-format)

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
    ${BREAKPAD_BIN}/src/libbreakpad.a
    ${BREAKPAD_BIN}/src/third_party/libdisasm/libdisasm.a
    ${CURL_LIBRARIES}
    ${ZIP_LIBRARIES}
    ${JSON_LIBRARIES}
)

target_link_libraries(
    stackwalker
    ${BREAKPAD_BIN}/src/libbreakpad.a
    ${BREAKPAD_BIN}/src/third_party/libdisasm/libdisasm.a
    ${CURL_LIBRARIES}
    ${ZIP_LIBRARIES}
    ${JSON_LIBRARIES}
)
