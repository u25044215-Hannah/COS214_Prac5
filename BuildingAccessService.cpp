#include "BuildingAccessService.h"
#include <iostream>
#include <utility>

BuildingAccessService::BuildingAccessService(std::unique_ptr<AccessController> controller)
    : controller_(std::move(controller)) {}
BuildingAccessService::~BuildingAccessService() {}

void BuildingAccessService::addArea(const std::string& id, const std::string& building,
                                    const std::string& name, AreaKind kind) {
    Area a;
    a.id = id; a.building = building; a.name = name; a.kind = kind; a.level = LEVEL_OPEN;
    areas_[id] = a;
}

bool BuildingAccessService::apply(const std::string& id, AccessLevel target) {
    std::map<std::string, Area>::iterator it = areas_.find(id);
    if (it == areas_.end()) {
        std::cout << "  [Access] FAILED: unknown area '" << id << "'\n";
        return false;
    }
    try {
        if (target == LEVEL_LOCKED) controller_->lock(id);
        else if (target == LEVEL_OPEN) controller_->unlock(id);
        else controller_->restrictToStaff(id);
    } catch (const AccessFault& e) {
        std::cout << "  [Access] FAILED: " << e.what() << "\n";
        return false;
    }
    it->second.level = target;
    std::cout << "  [Access] " << it->second.name << " (" << id << ") -> "
              << toString(target) << "\n";
    return true;
}

bool BuildingAccessService::lockArea(const std::string& id)     { return apply(id, LEVEL_LOCKED); }
bool BuildingAccessService::unlockArea(const std::string& id)   { return apply(id, LEVEL_OPEN); }
bool BuildingAccessService::restrictArea(const std::string& id) { return apply(id, LEVEL_RESTRICTED); }

AccessLevel BuildingAccessService::levelOf(const std::string& id) const {
    std::map<std::string, Area>::const_iterator it = areas_.find(id);
    if (it == areas_.end()) throw AccessFault("unknown area '" + id + "'");
    return it->second.level;
}

std::vector<Area> BuildingAccessService::areasInBuilding(const std::string& building) const {
    std::vector<Area> out;
    for (std::map<std::string, Area>::const_iterator it = areas_.begin(); it != areas_.end(); ++it)
        if (it->second.building == building) out.push_back(it->second);
    return out;
}
