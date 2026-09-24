#include "LegacyDoorPanel.h"
#include <iostream>

LegacyDoorPanel::LegacyDoorPanel() {
    int z[] = {101, 102, 103, 201, 202};
    for (int i = 0; i < 5; ++i) zones_[z[i]] = 0;
}

int LegacyDoorPanel::execute(int opcode, int zoneNo) {
    std::map<int, int>::iterator it = zones_.find(zoneNo);
    if (it == zones_.end()) return -2;
    std::map<int, bool>::const_iterator off = offline_.find(zoneNo);
    if (off != offline_.end() && off->second) return -3;
    if (opcode < 1 || opcode > 3) return -1;
    static const int resultState[] = {0, 1, 0, 2};
    it->second = resultState[opcode];
    std::cout << "    [LegacyPanel] EXEC op=" << opcode << " zone=" << zoneNo << "\n";
    return 0;
}

int LegacyDoorPanel::queryZone(int zoneNo) const {
    std::map<int, int>::const_iterator it = zones_.find(zoneNo);
    return it == zones_.end() ? -2 : it->second;
}

void LegacyDoorPanel::setZoneOffline(int zoneNo, bool offline) {
    offline_[zoneNo] = offline;
}
