#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include <windows.h>
#include <string>
#include <vector>
#include "FileCriteria.h"

class FileMonitor {
public:
    FileMonitor(std::wstring  directory, const std::vector<FileCriteria>& criteriaList);
    ~FileMonitor();
    bool StartMonitoring();
    void MonitorChanges();

private:
    HANDLE hDir;
    HANDLE hEvent;
    char buffer[4096];
    OVERLAPPED overlapped;
    std::wstring directory;
    std::vector<FileCriteria> criteriaList;

    void HandleFileChange(FILE_NOTIFY_INFORMATION* event);
    bool DeleteFileWithRetry(const std::wstring& filePath);
    std::wstring GetErrorMessage(DWORD errorCode);
};

#endif // FILEMONITOR_H
