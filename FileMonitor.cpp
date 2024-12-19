#include "FileMonitor.h"
#include <iostream>
#include <utility>

FileMonitor::FileMonitor(std::wstring  directory, const std::vector<FileCriteria>& criteriaList)
        : directory(std::move(directory)), criteriaList(criteriaList), hDir(INVALID_HANDLE_VALUE), hEvent(INVALID_HANDLE_VALUE) {
    overlapped = { 0 };
}

FileMonitor::~FileMonitor() {
    if (hEvent != INVALID_HANDLE_VALUE) {
        CloseHandle(hEvent);
    }
    if (hDir != INVALID_HANDLE_VALUE) {
        CloseHandle(hDir);
    }
}

bool FileMonitor::StartMonitoring() {
    hDir = CreateFileW(
            directory.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open directory: " << directory << std::endl;
        return false;
    }

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to create event" << std::endl;
        return false;
    }

    overlapped.hEvent = hEvent;
    return true;
}

void FileMonitor::MonitorChanges() {
    DWORD bytesReturned;

    while (true) {
        if (!ReadDirectoryChangesW(
                hDir,
                buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME,
                &bytesReturned,
                &overlapped,
                NULL
        )) {
            std::wcerr << L"Error while monitoring changes" << std::endl;
            break;
        }

        WaitForSingleObject(hEvent, INFINITE);
        ResetEvent(hEvent);

        auto* event = (FILE_NOTIFY_INFORMATION*)buffer;
        do {
            HandleFileChange(event);

            if (event->NextEntryOffset == 0) {
                break;
            }
            event = (FILE_NOTIFY_INFORMATION*)((BYTE*)event + event->NextEntryOffset);
        } while (true);
    }
}

void FileMonitor::HandleFileChange(FILE_NOTIFY_INFORMATION* event) {
    if (event->Action == FILE_ACTION_ADDED) {
        std::wstring fileName(event->FileName, event->FileNameLength / sizeof(WCHAR));
        std::wstring fullPath = directory + L"\\" + fileName;

        for (const auto& criteria : criteriaList) {
            if (criteria.Matches(fileName)) {
                std::wstring outputFile = L"New file: " + fileName + L"\n";

                DWORD written;
                WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), outputFile.c_str(), outputFile.size(), &written, NULL);

                if (DeleteFileWithRetry(fullPath)) {
                    std::wstring deleteMessage = L"File deleted: " + fileName + L"\n";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), deleteMessage.c_str(), deleteMessage.size(), &written, NULL);
                } else {
                    std::wstring errorMessage = L"Failed to delete file: " + fileName + L"\n";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), errorMessage.c_str(), errorMessage.size(), &written, NULL);
                }
                break;
            }
        }
    }
}

bool FileMonitor::DeleteFileWithRetry(const std::wstring& filePath) {
    const int maxRetries = 5;
    const int retryDelay = 100; // ms

    for (int attempt = 0; attempt < maxRetries; ++attempt) {
        if (DeleteFileW(filePath.c_str())) {
            return true;
        }

        DWORD errorCode = GetLastError();
        if (errorCode != ERROR_SHARING_VIOLATION && errorCode != ERROR_ACCESS_DENIED) {
            std::wstring errorMessage = L"Error deleting file: " + GetErrorMessage(errorCode);
            WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), errorMessage.c_str(), errorMessage.size(), NULL, NULL);
            return false;
        }

        Sleep(retryDelay);
    }

    DWORD errorCode = GetLastError();
    std::wstring errorMessage = L"Error deleting file after retries: " + GetErrorMessage(errorCode);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), errorMessage.c_str(), errorMessage.size(), NULL, NULL);
    return false;
}

std::wstring FileMonitor::GetErrorMessage(DWORD errorCode) {
    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    std::wstring message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
}
