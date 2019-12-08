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

#ifndef BREAKDOWN_COMMON_H
#define BREAKDOWN_COMMON_H

#include <QString>
#include <QFile>
#include <QDir>

#include <vector>
#include <string>

class BreakDownCommon
{
public:
    static const QString localPath(const QString &folder)
    {
        QString path = QString("%1/.config/FxArena/breakdown/%2").arg(QDir::homePath().arg(folder));
        if (!QFile::exists(path)) {
            QDir dir(path);
            dir.mkpath(path);
        }
        return path;
    }
    static const std::vector<std::string> localStorage()
    {
        std::vector<std::string> result;

        result.push_back(localPath("symbols").toStdString());
        result.push_back(localPath("cache").toStdString());

        return result;
    }
    static const std::vector<std::string> remoteStorage()
    {
        std::vector<std::string> result;

        //result.push_back("https://sourceforge.net/projects/natron/files/symbols");
        //result.push_back("https://sourceforge.net/projects/openfx-arena/files/symbols");
        result.push_back("https://stackwalker.000webhostapp.com/symbols");

        return result;
    }
    static const std::string cacheStorage()
    {
        return localPath("cache").toStdString();
    }
};

#endif // BREAKDOWN_COMMON_H
