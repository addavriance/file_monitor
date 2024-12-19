#ifndef FILECRITERIA_H
#define FILECRITERIA_H

#include <string>

class FileCriteria {
public:
    FileCriteria(std::wstring  namePattern, std::wstring  extensionPattern);

    bool Matches(const std::wstring& fileName) const;

private:
    std::wstring namePattern;
    std::wstring extensionPattern;

    static bool MatchPattern(const std::wstring& pattern, const std::wstring& value) ;
};

#endif // FILECRITERIA_H
