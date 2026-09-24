#ifndef LOCKDOWNFACADE_H
#define LOCKDOWNFACADE_H

#include "AuditLog.h"
#include "BuildingAccessService.h"
#include "NotificationService.h"
#include <string>
#include <vector>

struct LockdownReport {
    int secured;
    std::vector<std::string> failed;
    LockdownReport() : secured(0) {}
    bool complete() const { return failed.empty(); }
};

// FACADE: single entry point for the building-lockdown workflow.
// Ownership: holds NON-owning references; subsystems stay independently usable.
class LockdownFacade {
public:
    LockdownFacade(BuildingAccessService& access, NotificationService& notifier, AuditLog& audit);
    virtual ~LockdownFacade();

    // entrances -> locked, interior -> staff-only, broadcast, audit
    LockdownReport initiateLockdown(const std::string& buildingId, const std::string& incidentRef);
    // reopen everything, all-clear broadcast, audit
    LockdownReport liftLockdown(const std::string& buildingId, const std::string& incidentRef);

private:
    BuildingAccessService& access_;
    NotificationService& notifier_;
    AuditLog& audit_;
};

#endif
