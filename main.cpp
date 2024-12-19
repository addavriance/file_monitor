#include <iostream>
#include <vector>
#include "FileMonitor.h"
#include "FileCriteria.h"

int main() {
    std::vector<FileCriteria> criteriaList = {
            FileCriteria(L"fba", L".json"), // put your blacklisted files here
    };

    FileMonitor fileMonitor(L"C:\\", criteriaList); // directory that will be monitored for blacklisted files
    if (!fileMonitor.StartMonitoring()) {
        std::cerr << "Failed to start monitoring" << std::endl;
        return 1;
    }

    std::cout << "Monitoring started" << std::endl;

    fileMonitor.MonitorChanges();
    return 0;
}
