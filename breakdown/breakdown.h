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

#ifndef BREAKDOWN_H
#define BREAKDOWN_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QString>
#include <QUrl>

namespace Ui {
class BreakDown;
}

class BreakDown : public QMainWindow
{
    Q_OBJECT

public:
    explicit BreakDown(QWidget *parent = nullptr);
    ~BreakDown();

signals:
    void parseDumpFinished(const QString json, const QString uuid, bool failed);

private slots:
    void loadSettings();
    void saveSettings();
    void setupWidgets();
    void setLineEditCursorPosition();
    void clearReport();
    void openJsonFile(const QString &file);
    void openJsonString(const QString json, const QString customID = QString());
    void openDumpFile(const QString &file);
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void on_reportInfoCrashTree_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionAbout_triggered();
    void handleParseDumpFinished(const QString json, const QString uuid, bool failed);
    //void downloadReportXML(const QUrl &url);
    //void downloadReportXMLFinished();

private:
    Ui::BreakDown *ui;
};

#endif // BREAKDOWN_H
