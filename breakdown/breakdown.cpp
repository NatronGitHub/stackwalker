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
#include "ui_breakdown.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDesktopServices>
#include <QMessageBox>
#include <QSettings>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include "stackwalker.h"

BreakDown::BreakDown(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BreakDown)
{
    ui->setupUi(this);
    setWindowTitle(QString("Breakdown %1")
                   .arg(BREAKDOWN_VERSION));

    setupWidgets();
    loadSettings();
}

BreakDown::~BreakDown()
{
    saveSettings();
    delete ui;
}

void BreakDown::loadSettings()
{

}

void BreakDown::saveSettings()
{

}

void BreakDown::setupWidgets()
{
    ui->contactReply->setHidden(true);
    ui->reportUuid->setReadOnly(true);
    ui->reportTimestamp->setReadOnly(true);
    ui->reportComment->setReadOnly(true);
    ui->reportGitCommit->setReadOnly(true);
    ui->reportGitCommitIO->setReadOnly(true);
    ui->reportGitCommitMisc->setReadOnly(true);
    ui->reportGitCommitArena->setReadOnly(true);
    ui->reportGitCommitGmic->setReadOnly(true);
    ui->reportGitBranch->setReadOnly(true);
    ui->reportsOS->setReadOnly(true);
    ui->reportProduct->setReadOnly(true);
    ui->reportCrashType->setReadOnly(true);
    ui->reportCpuArch->setReadOnly(true);
    ui->reportClientIP->setReadOnly(true);
    ui->reportContact->setReadOnly(true);
    ui->reportSeverity->setReadOnly(true);
    ui->glRenderer->setReadOnly(true);
    ui->GLversion->setReadOnly(true);
    ui->GLvendor->setReadOnly(true);
    ui->GLShader->setReadOnly(true);
    ui->GLext->setReadOnly(true);

    // we don't need to show these
    ui->reportGitCommit->setHidden(true);
    ui->reportGitCommitIO->setHidden(true);
    ui->reportGitCommitMisc->setHidden(true);
    ui->reportGitCommitArena->setHidden(true);
    ui->reportGitCommitGmic->setHidden(true);
    ui->reportGitBranch->setHidden(true);
    ui->reportClientIP->setHidden(true);
    ui->reportCpuArch->setHidden(true);
    ui->reportTimestamp->setHidden(true);

    ui->commitLabel->setHidden(true);
    ui->ioLabel->setHidden(true);
    ui->miscLabel->setHidden(true);
    ui->arenaLabel->setHidden(true);
    ui->gmicLabel->setHidden(true);
    ui->branchLabel->setHidden(true);
    ui->locationLabel->setHidden(true);
    ui->cpuLabel->setHidden(true);
    ui->timeLabel->setHidden(true);

    connect(this,
            SIGNAL(parseDumpFinished(QString,QString,bool)),
            this,
            SLOT(handleParseDumFinished(QString,QString,bool)));
}

void BreakDown::setLineEditCursorPosition()
{
    ui->reportUuid->setCursorPosition(0);
    ui->reportTimestamp->setCursorPosition(0);
    ui->reportGitCommit->setCursorPosition(0);
    ui->reportGitCommitIO->setCursorPosition(0);
    ui->reportGitCommitMisc->setCursorPosition(0);
    ui->reportGitCommitArena->setCursorPosition(0);
    ui->reportGitCommitGmic->setCursorPosition(0);
    ui->reportGitBranch->setCursorPosition(0);
    ui->reportsOS->setCursorPosition(0);
    ui->reportProduct->setCursorPosition(0);
    ui->reportCrashType->setCursorPosition(0);
    ui->reportCpuArch->setCursorPosition(0);
    ui->reportClientIP->setCursorPosition(0);
    ui->reportContact->setCursorPosition(0);
    ui->reportSeverity->setCursorPosition(0);
    ui->glRenderer->setCursorPosition(0);
    ui->GLversion->setCursorPosition(0);
    ui->GLvendor->setCursorPosition(0);
    ui->GLShader->setCursorPosition(0);
}

void BreakDown::clearReport()
{
    ui->reportUuid->clear();
    ui->reportTimestamp->clear();
    ui->reportComment->clear();
    ui->reportGitCommit->clear();
    ui->reportGitCommitIO->clear();
    ui->reportGitCommitMisc->clear();
    ui->reportGitCommitArena->clear();
    ui->reportGitCommitGmic->clear();
    ui->reportGitBranch->clear();
    ui->reportsOS->clear();
    ui->reportProduct->clear();
    ui->reportCrashType->clear();
    ui->reportCpuArch->clear();
    ui->reportClientIP->clear();
    ui->reportContact->clear();
    ui->reportSeverity->clear();
    ui->glRenderer->clear();
    ui->GLversion->clear();
    ui->GLvendor->clear();
    ui->GLShader->clear();
    ui->GLext->setPlainText(QString());
    ui->reportInfoCrashTree->clear();
    ui->reportInfoThreadTree->clear();
    ui->reportInfoModulesTree->clear();
}

void BreakDown::openJsonFile(const QString &file)
{
    if (file.isEmpty()) { return; }
    QFile jsonFile(file);
    if (!jsonFile.open(QIODevice::ReadOnly|QIODevice::Text)) { return; }
    QString rawJson = jsonFile.readAll();
    jsonFile.close();
    if (!rawJson.isEmpty()) { openJsonString(rawJson); }
}

void BreakDown::openJsonString(const QString json, const QString customID)
{
    ui->actionOpen->setDisabled(false);
    ui->statusBar->showMessage(tr("Done"));

    if (json.isEmpty()) { return; }
    QJsonDocument doc(QJsonDocument::fromJson(json.toUtf8()));
    if (doc.isEmpty() || doc.isNull()) { return; }
    QJsonObject obj = doc.object();

    // clear old info
    clearReport();

    // basic info
    QJsonValue crash_info = obj.value(QString("crash_info"));
    QJsonObject crash_info_item = crash_info.toObject();
    QJsonValue system_info = obj.value(QString("system_info"));
    QJsonObject system_info_item = system_info.toObject();

    QString uuid = obj.value(QString("uuid")).toString();
    if (uuid.isEmpty() && !customID.isEmpty()) { uuid = customID; }
    QString timestamp = obj.value(QString("submitted_timestamp")).toString();
    QString comment = obj.value(QString("Comments")).toString();
    QString git_commit = obj.value(QString("git_commit")).toString();
    QString io_commit = obj.value(QString("io_commit")).toString();
    QString misc_commit = obj.value(QString("misc_commit")).toString();
    QString arena_commit = obj.value(QString("arena_commit")).toString();
    QString git_branch = obj.value(QString("git_branch")).toString();
    QString linux_distro = obj.value(QString("linux_distro")).toString();
    QString product = obj.value(QString("ProductName")).toString();
    QString product_version = obj.value(QString("Version")).toString();
    QString crash_type = crash_info_item["type"].toString();
    QString cpu_arch = system_info_item["cpu_arch"].toString();
    QString cpu_info = system_info_item["cpu_info"].toString();
    QString crash_os = system_info_item["os"].toString();
    QString os_version = system_info_item["os_ver"].toString();
    QString client_ip = obj.value(QString("client_ip")).toString();
    QString user_contact = obj.value(QString("user_contact")).toString();
    QString user_severity = obj.value(QString("user_severity")).toString();
    QString gl_renderer = obj.value(QString("gl_renderer")).toString();
    QString gl_version = obj.value(QString("gl_version")).toString();
    QString gl_vendor = obj.value(QString("gl_vendor")).toString();
    QString gl_shader = obj.value(QString("gl_shader")).toString();
    QString gl_ext = obj.value(QString("gl_ext")).toString();
    //QString features = obj.value(QString("features")).toString();

    ui->reportUuid->setText(uuid);
    ui->reportTimestamp->setText(timestamp);
    ui->reportComment->setPlainText(comment);
    ui->reportGitCommit->setText(git_commit);
    ui->reportGitCommitIO->setText(io_commit);
    ui->reportGitCommitMisc->setText(misc_commit);
    ui->reportGitCommitArena->setText(arena_commit);
    ui->reportGitCommitGmic->setText(QString()); // TODO
    ui->reportGitBranch->setText(git_branch);
    if (linux_distro.isEmpty()) {
        ui->reportsOS->setText(QString("%1 %2")
                               .arg(crash_os)
                               .arg(os_version));
    } else {
        ui->reportsOS->setText(QString("%1 (%2) %3")
                               .arg(linux_distro)
                               .arg(crash_os)
                               .arg(os_version));
    }
    ui->reportProduct->setText(QString("%1 %2")
                               .arg(product)
                               .arg(product_version));
    ui->reportCrashType->setText(crash_type);
    ui->reportCpuArch->setText(QString("%1 (%2)")
                               .arg(cpu_arch)
                               .arg(cpu_info));
    ui->reportClientIP->setText(client_ip);
    ui->reportContact->setText(user_contact);
    ui->reportSeverity->setText(user_severity);
    ui->glRenderer->setText(gl_renderer);
    ui->GLversion->setText(gl_version);
    ui->GLvendor->setText(gl_vendor);
    ui->GLShader->setText(gl_shader);
    ui->GLext->setPlainText(gl_ext);

    setLineEditCursorPosition(); // make sure we start at 0

    // crash thread
    QJsonValue crashing_thread = obj.value(QString("crashing_thread"));
    QJsonObject crashing_thread_item = crashing_thread.toObject();
    QJsonArray crashing_thread_item_frames = crashing_thread_item["frames"].toArray();
    foreach (const QJsonValue &value, crashing_thread_item_frames) {
        QJsonObject output = value.toObject();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->reportInfoCrashTree);
        item->setText(0, output["function"].toString());
        item->setData(0, Qt::ToolTipRole, output["function"].toString());
        item->setText(1, output["function_offset"].toString());
        item->setText(2, output["module_offset"].toString());
        item->setText(3, QString::number(output["frame"].toInt()));
        item->setText(4, output["module"].toString());
        item->setText(5, output["offset"].toString());
        item->setText(6, output["trust"].toString());

        QString fileName = output["file"].toString();
        QStringList splitFile;
        QString fileUrl;
        if (fileName.contains("../..")) {
            splitFile = fileName.split("../..");

        } else if (fileName.contains("..\\..")) {
            splitFile = fileName.split("..\\..");
        }
        if (splitFile.count()==2) {
            fileName = splitFile.at(1);
        }
        if (fileName.startsWith("/Natron/")) { // gen url to Natron git repo
            fileName = fileName.replace("/Natron/", "");
            fileUrl = QString("https://github.com/NatronGitHub/Natron/blob/%1/%2#L%3")
                      .arg(git_commit.isEmpty()?"RB-2.3":git_commit) // RB-2.3 is fallback
                      .arg(fileName)
                      .arg(QString::number(output["line"].toInt()));

        }
        if (!fileName.isEmpty()) {
            fileName.append(QString(":%1").arg(QString::number(output["line"].toInt())));
        }
        item->setText(7, fileName);
        if (!fileUrl.isEmpty()) {
            item->setData(7, Qt::ToolTipRole, fileUrl);
        }
        ui->reportInfoCrashTree->addTopLevelItem(item);
    }

    // threads
    QJsonArray threads_item_frames = obj["threads"].toArray();
    foreach (const QJsonValue &value, threads_item_frames) {
        QJsonObject output = value.toObject();
        foreach (const QJsonValue &value2, output["frames"].toArray()) {
            QJsonObject output2 = value2.toObject();
            QString missing_symbols;
            if (output2["missing_symbols"].toBool()) { missing_symbols = "yes"; }
            else { missing_symbols = "no"; }
            QTreeWidgetItem *item  = new QTreeWidgetItem(ui->reportInfoThreadTree);
            item->setText(0, output2["function"].toString());
            item->setText(1, output2["function_offset"].toString());
            item->setText(2, output2["module_offset"].toString());
            item->setText(3, QString::number(output2["frame"].toInt()));
            item->setText(4, output2["module"].toString());
            item->setText(5, output2["offset"].toString());
            item->setText(6, output2["trust"].toString());
            item->setText(7, missing_symbols);
            item->setText(8, output2["file"].toString());
            //item->setText(5, QString::number(output2["line"].toInt())); // TODO
            ui->reportInfoThreadTree->addTopLevelItem(item);
        }
    }

    // modules
    QJsonArray modules = obj["modules"].toArray();
    foreach (const QJsonValue &value, modules) {
        QJsonObject output = value.toObject();
        QString loaded_symbols;
        if (output["loaded_symbols"].toBool()) { loaded_symbols = "yes"; }
        else { loaded_symbols = "no"; }
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->reportInfoModulesTree);
        item->setText(0, output["base_addr"].toString());
        item->setText(1, output["code_id"].toString());
        item->setText(2, output["filename"].toString());
        item->setText(3, output["debug_file"].toString());
        item->setText(4, loaded_symbols);
        item->setText(5, output["version"].toString());
        item->setText(6, output["end_addr"].toString());
        item->setText(7, output["debug_id"].toString());
        ui->reportInfoModulesTree->addTopLevelItem(item);
    }
}

void BreakDown::openDumpFile(const QString &file)
{
    vector<string> server_path;
    vector<string> symbol_paths;

    // TODO: settings (also remember cache and tmp!!!)

    //server_path.push_back("https://sourceforge.net/projects/natron/files/symbols");
    server_path.push_back("https://sourceforge.net/projects/openfx-arena/files/symbols");
    //server_path.push_back("https://stackwalker.000webhostapp.com/symbols");

    Minidump minidump(file.toStdString());
    minidump.Read();

    Json::Value root;
    scoped_ptr<SymbolSupplier> symbol_supplier;
    HTTPSymbolSupplier* http_symbol_supplier = nullptr;
    if (!server_path.empty()) {
        http_symbol_supplier = new HTTPSymbolSupplier(server_path,
                                                      QDir::tempPath().toStdString(),
                                                      symbol_paths,
                                                      QDir::tempPath().toStdString());
        symbol_supplier.reset(http_symbol_supplier);
    } else if (!symbol_paths.empty()) {
        symbol_supplier.reset(new SimpleSymbolSupplier(symbol_paths));
    }

    BasicSourceLineResolver resolver;
    StackFrameSymbolizerForward symbolizer(symbol_supplier.get(), &resolver);
    MinidumpProcessor minidump_processor(&symbolizer, true);
    ProcessState process_state;
    ProcessResult result = minidump_processor.Process(&minidump, &process_state);

    if (result != google_breakpad::PROCESS_OK) {
        string failed = ResultString(result);
        emit parseDumpFinished(QString::fromStdString(failed), file, true);
        return;
    }

    Json::Value raw_root(Json::objectValue);

    root["status"] = ResultString(result);
    root["sensitive"] = Json::Value(Json::objectValue);
    if (result == google_breakpad::PROCESS_OK) {
        ConvertProcessStateToJSON(process_state,
                                  symbolizer,
                                  http_symbol_supplier,
                                  root,
                                  raw_root);
    }
    ConvertMemoryInfoToJSON(minidump, raw_root, root);

    // Get the PID.
    MinidumpMiscInfo* misc_info = minidump.GetMiscInfo();
    if (misc_info && misc_info->misc_info() &&
        (misc_info->misc_info()->flags1 & MD_MISCINFO_FLAGS1_PROCESS_ID))
    {
        root["pid"] = misc_info->misc_info()->process_id;
    }

    // See if this is a Linux dump with /proc/cpuinfo in it
    uint32_t cpuinfo_length = 0;
    if (process_state.system_info()->os == "Linux" &&
        minidump.SeekToStreamType(MD_LINUX_CPU_INFO, &cpuinfo_length))
    {
        string contents;
        contents.resize(cpuinfo_length);
        if (minidump.ReadBytes(const_cast<char*>(contents.data()), cpuinfo_length)) {
            ConvertCPUInfoToJSON(contents, root);
        }
    }

    // See if this is a Linux dump with /etc/lsb-release in it
    uint32_t length = 0;
    if (process_state.system_info()->os == "Linux" &&
        minidump.SeekToStreamType(MD_LINUX_LSB_RELEASE, &length))
    {
        string contents;
        contents.resize(length);
        if (minidump.ReadBytes(const_cast<char*>(contents.data()), length)) {
            ConvertLSBReleaseToJSON(contents, root);
        }
    }

    scoped_ptr<Json::Writer> writer;
    writer.reset(new Json::StyledWriter());
    string json = writer->write(root);

    if (!json.empty()) {
        QFileInfo info(file);
        //openJsonString(QString::fromStdString(json), info.baseName());
        emit parseDumpFinished(QString::fromStdString(json), info.baseName(), false);
    }
}

void BreakDown::on_actionQuit_triggered()
{
    qApp->quit();
}

void BreakDown::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open Stackwalker JSON or Breakpad DMP"),
                                                QDir::homePath(),
                                                "*.json *.dmp");
    if (file.isEmpty()) { return; }

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(file);

    ui->actionOpen->setDisabled(true);
    ui->statusBar->showMessage(tr("Parsing minidump, this might take a while ..."), -1);
    if (type.name() == "application/json") {
        openJsonFile(file);
    } else {
        QtConcurrent::run(this, &BreakDown::openDumpFile, file);
    }
}

void BreakDown::on_reportInfoCrashTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->data(7, Qt::ToolTipRole).isNull()) { return; }
    QDesktopServices::openUrl(QUrl(item->data(7, Qt::ToolTipRole).toString()));
}

void BreakDown::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       QString("%1 Breakdown").arg(tr("About")),
                       QString("<h3>Breakdown %1</h3>"
                       "<p>Parse crash reports from Breakpad.<p>"
                       "<p>Copyright &copy;2019 <a href=\"https://github.com/rodlie\">Ole-André Rodlie</a>.</p>")
                       .arg(BREAKDOWN_VERSION));
}

void BreakDown::handleParseDumFinished(const QString json,
                                       const QString uuid,
                                       bool failed)
{
    ui->actionOpen->setDisabled(false);
    ui->statusBar->showMessage(tr("Done"));
    if (failed) { return; }
    openJsonString(json, uuid);
}
