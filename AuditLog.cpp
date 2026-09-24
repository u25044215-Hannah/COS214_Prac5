#include "AuditLog.h"
#include <iostream>

void AuditLog::record(const std::string& entry) {
    entries_.push_back(entry);
    std::cout << "  [Audit #" << entries_.size() << "] " << entry << "\n";
}

const std::vector<std::string>& AuditLog::entries() const { return entries_; }
