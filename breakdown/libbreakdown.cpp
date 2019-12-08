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

#include "libbreakdown.h"
#include "stackwalker.h"

const std::string LibBreakDown::parseDumpFile(const std::string &fileName,
                                              const std::vector<std::string> &localStorage,
                                              const std::vector<std::string> &remoteStorage,
                                              const std::string &cache)
{
    Minidump minidump(fileName);
    minidump.Read();

    Json::Value root;
    scoped_ptr<SymbolSupplier> symbol_supplier;
    HTTPSymbolSupplier* http_symbol_supplier = nullptr;
    if (!remoteStorage.empty()) {
        http_symbol_supplier = new HTTPSymbolSupplier(remoteStorage,
                                                      cache,
                                                      localStorage,
                                                      cache);
        symbol_supplier.reset(http_symbol_supplier);
    } else if (!localStorage.empty()) {
        symbol_supplier.reset(new SimpleSymbolSupplier(localStorage));
    }

    BasicSourceLineResolver resolver;
    StackFrameSymbolizerForward symbolizer(symbol_supplier.get(), &resolver);
    MinidumpProcessor minidump_processor(&symbolizer, true);
    ProcessState process_state;
    ProcessResult result = minidump_processor.Process(&minidump, &process_state);

    if (result != google_breakpad::PROCESS_OK) {
        //string failed = ResultString(result);
        return std::string();
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

    return json;
}
