#ifndef LEGACYDOORPANEL_H
#define LEGACYDOORPANEL_H

#include <map>

// ADAPTER: Adaptee - legacy third-party door panel driver. Pretend we cannot change it.
// Talks in numeric zones, numeric opcodes and integer return codes.
//   opcode : 1 = lock, 2 = unlock, 3 = card-only
//   execute: 0 ok, -1 bad opcode, -2 unknown zone, -3 zone offline
//   queryZone: 0 open, 1 locked, 2 card-only, -2 unknown zone
class LegacyDoorPanel {
public:
    LegacyDoorPanel();
    int execute(int opcode, int zoneNo);
    int queryZone(int zoneNo) const;
    void setZoneOffline(int zoneNo, bool offline); // simulate hardware fault
private:
    std::map<int, int> zones_;
    std::map<int, bool> offline_;
};

#endif
