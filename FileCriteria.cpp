#include "FileCriteria.h"
#include <utility>

FileCriteria::FileCriteria(std::wstring  namePattern, std::wstring  extensionPattern)
        : namePattern(std::move(namePattern)), extensionPattern(std::move(extensionPattern)) {}

bool FileCriteria::Matches(const std::wstring& fileName) const {
    std::wstring name, extension;
    size_t dotPos = fileName.rfind(L'.');
    if (dotPos != std::wstring::npos) {
        name = fileName.substr(0, dotPos);
        extension = fileName.substr(dotPos);
    } else {
        name = fileName;
        extension = L"";
    }

    return MatchPattern(namePattern, name) && MatchPattern(extensionPattern, extension);
}

bool FileCriteria::MatchPattern(const std::wstring& pattern, const std::wstring& value) {
    if (pattern == L"*") return true;
    return value.find(pattern) != std::wstring::npos;
}
