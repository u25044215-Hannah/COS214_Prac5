#ifndef AUDITLOG_H
#define AUDITLOG_H

#include <string>
#include <vector>

// Subsystem service: append-only record of operator/system actions.
class AuditLog {
public:
    void record(const std::string& entry);
    const std::vector<std::string>& entries() const;
private:
    std::vector<std::string> entries_;
};

#endif
