/*
* Breakdown <https://github.com/NatronGitHub/stackwalker>
* Copyright (C) 2019 Ole-André Rodlie
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*
*/

#ifndef LIBBREAKDOWN_H
#define LIBBREAKDOWN_H

#include <string>
#include <vector>

#if defined(_WIN32)
#  if !defined(BREAKDOWN_EXPORT) && !defined(BREAKDOWN_IMPORT)
#    define BREAKDOWN_EXPORT
#  elif defined(BREAKDOWN_IMPORT)
#    if defined(BREAKDOWN_EXPORT)
#      undef BREAKDOWN_EXPORT
#    endif
#    define BREAKDOWN_EXPORT __declspec(dllimport)
#  elif defined(BREAKDOWN_EXPORT)
#    undef BREAKDOWN_EXPORT
#    define BREAKDOWN_EXPORT __declspec(dllexport)
#  endif
#else
#  define BREAKDOWN_EXPORT
#endif

class BREAKDOWN_EXPORT LibBreakDown
{

public:
    /** @brief Convert Breakpad dump file to JSON */
    static const std::string parseDumpFile(const std::string &fileName,
                                           const std::vector<std::string> &localStorage,
                                           const std::vector<std::string> &remoteStorage,
                                           const std::string &cache);
};

#endif // LIBBREAKDOWN_H
