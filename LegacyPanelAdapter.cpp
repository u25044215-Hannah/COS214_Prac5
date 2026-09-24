#include "LegacyPanelAdapter.h"
#include <sstream>

LegacyPanelAdapter::LegacyPanelAdapter(LegacyDoorPanel& panel) : panel_(panel) {}
LegacyPanelAdapter::~LegacyPanelAdapter() {}

void LegacyPanelAdapter::mapArea(const std::string& areaId, int zoneNo) {
    zoneMap_[areaId] = zoneNo;
}

int LegacyPanelAdapter::zoneFor(const std::string& areaId) const {
    std::map<std::string, int>::const_iterator it = zoneMap_.find(areaId);
    if (it == zoneMap_.end())
        throw AccessFault("no legacy zone mapped for area '" + areaId + "'");
    return it->second;
}

static std::string describe(int rc, const std::string& areaId, int zone) {
    std::ostringstream os;
    os << "legacy panel rejected request for '" << areaId << "' (zone " << zone << "): ";
    if (rc == -1) os << "bad opcode";
    else if (rc == -2) os << "unknown zone";
    else if (rc == -3) os << "zone offline";
    else os << "error " << rc;
    return os.str();
}

void LegacyPanelAdapter::send(int opcode, const std::string& areaId) {
    int zone = zoneFor(areaId);
    int rc = panel_.execute(opcode, zone);
    if (rc != 0) throw AccessFault(describe(rc, areaId, zone));
}

void LegacyPanelAdapter::lock(const std::string& areaId)            { send(OP_LOCK, areaId); }
void LegacyPanelAdapter::unlock(const std::string& areaId)          { send(OP_UNLOCK, areaId); }
void LegacyPanelAdapter::restrictToStaff(const std::string& areaId) { send(OP_CARD_ONLY, areaId); }

AccessLevel LegacyPanelAdapter::statusOf(const std::string& areaId) const {
    int zone = zoneFor(areaId);
    int rc = panel_.queryZone(zone);
    if (rc == 0) return LEVEL_OPEN;
    if (rc == 1) return LEVEL_LOCKED;
    if (rc == 2) return LEVEL_RESTRICTED;
    throw AccessFault(describe(rc, areaId, zone));
}
