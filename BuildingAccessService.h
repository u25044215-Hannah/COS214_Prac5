#ifndef BUILDINGACCESSSERVICE_H
#define BUILDINGACCESSSERVICE_H

#include "AccessController.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

enum AreaKind { AREA_ENTRANCE, AREA_INTERIOR };

struct Area {
    std::string id;
    std::string building;
    std::string name;
    AreaKind kind;
    AccessLevel level;
};

// Subsystem service: campus areas + lock/unlock/restrict. Talks to hardware only
// through the AccessController target interface (never to the legacy panel).
// Ownership: OWNS the controller (unique_ptr) -> adapter destroyed with the service.
class BuildingAccessService {
public:
    explicit BuildingAccessService(std::unique_ptr<AccessController> controller);
    ~BuildingAccessService();

    void addArea(const std::string& id, const std::string& building,
                 const std::string& name, AreaKind kind);

    // Return false (and log) on failure instead of failing silently.
    bool lockArea(const std::string& id);
    bool unlockArea(const std::string& id);
    bool restrictArea(const std::string& id);

    AccessLevel levelOf(const std::string& id) const;
    std::vector<Area> areasInBuilding(const std::string& building) const;

private:
    bool apply(const std::string& id, AccessLevel target);

    std::unique_ptr<AccessController> controller_;
    std::map<std::string, Area> areas_;
};

#endif
