#include "LockdownFacade.h"
#include <sstream>

LockdownFacade::LockdownFacade(BuildingAccessService& access, NotificationService& notifier,
                               AuditLog& audit)
    : access_(access), notifier_(notifier), audit_(audit) {}
LockdownFacade::~LockdownFacade() {}

static std::string joinIds(const std::vector<std::string>& v) {
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) s += (i ? ", " : "") + v[i];
    return s;
}

LockdownReport LockdownFacade::initiateLockdown(const std::string& buildingId,
                                                const std::string& incidentRef) {
    LockdownReport report;
    audit_.record("LOCKDOWN START building=" + buildingId + " incident=" + incidentRef);

    std::vector<Area> areas = access_.areasInBuilding(buildingId);
    if (areas.empty()) {
        report.failed.push_back(buildingId);
        audit_.record("LOCKDOWN ABORTED: unknown building " + buildingId);
        return report;
    }

    for (size_t i = 0; i < areas.size(); ++i) {
        bool ok = (areas[i].kind == AREA_ENTRANCE) ? access_.lockArea(areas[i].id)
                                                    : access_.restrictArea(areas[i].id);
        if (ok) ++report.secured; else report.failed.push_back(areas[i].id);
    }

    std::ostringstream msg;
    if (report.complete()) {
        msg << "LOCKDOWN in " << buildingId << ": all " << report.secured << " areas secured. Shelter in place.";
    } else {
        msg << "PARTIAL LOCKDOWN in " << buildingId << ": " << report.secured << " secured, manual check needed for "
            << joinIds(report.failed);
    }
    notifier_.broadcast("PA+SMS", msg.str());
    audit_.record("LOCKDOWN " + std::string(report.complete() ? "COMPLETE" : "PARTIAL") + " building=" + buildingId);
    return report;
}

LockdownReport LockdownFacade::liftLockdown(const std::string& buildingId,
                                            const std::string& incidentRef) {
    LockdownReport report;
    audit_.record("LOCKDOWN LIFT building=" + buildingId + " incident=" + incidentRef);
    std::vector<Area> areas = access_.areasInBuilding(buildingId);
    for (size_t i = 0; i < areas.size(); ++i) {
        if (access_.unlockArea(areas[i].id)) ++report.secured;
        else report.failed.push_back(areas[i].id);
    }
    if (report.complete()) {
        notifier_.broadcast("PA+SMS", "ALL CLEAR in " + buildingId);
        audit_.record("LOCKDOWN LIFTED building=" + buildingId);
    } else {
        notifier_.broadcast("PA+SMS", "PARTIAL RELEASE in " + buildingId + ": manual check needed for " + joinIds(report.failed));
        audit_.record("LOCKDOWN LIFT PARTIAL building=" + buildingId);
    }
    return report;
}
