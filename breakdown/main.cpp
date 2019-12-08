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

#include "breakdown.h"
#include <QApplication>
#include <QStringList>
#include <QFile>
#include "libbreakdown.h"
#include "common.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("breakdown");
    QApplication::setOrganizationName("FxArena");

    QStringList args = a.arguments();

    if (args.size()==2) {
        QString filename = args.at(1);
        if (QFile::exists(filename)) {
            std::string result = LibBreakDown::parseDumpFile(filename.toStdString(),
                                                             BreakDownCommon::localStorage(),
                                                             BreakDownCommon::remoteStorage(),
                                                             BreakDownCommon::cacheStorage());
            std::cout << result << std::endl;
            return 0;
        }
    }

    BreakDown w;
    w.show();

    return a.exec();
}
