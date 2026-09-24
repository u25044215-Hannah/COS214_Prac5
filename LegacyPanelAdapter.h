#ifndef LEGACYPANELADAPTER_H
#define LEGACYPANELADAPTER_H

#include "AccessController.h"
#include "LegacyDoorPanel.h"
#include <map>
#include <string>

// ADAPTER: Adapter. Translates area names -> zone numbers, actions -> opcodes,
// and integer return codes -> AccessFault exceptions / AccessLevel.
// Ownership: holds a NON-owning reference; the panel must outlive the adapter.
class LegacyPanelAdapter : public AccessController {
public:
    explicit LegacyPanelAdapter(LegacyDoorPanel& panel);
    virtual ~LegacyPanelAdapter();

    void mapArea(const std::string& areaId, int zoneNo);

    virtual void lock(const std::string& areaId);
    virtual void unlock(const std::string& areaId);
    virtual void restrictToStaff(const std::string& areaId);
    virtual AccessLevel statusOf(const std::string& areaId) const;

private:
    enum { OP_LOCK = 1, OP_UNLOCK = 2, OP_CARD_ONLY = 3 };
    int zoneFor(const std::string& areaId) const;
    void send(int opcode, const std::string& areaId);

    LegacyDoorPanel& panel_;
    std::map<std::string, int> zoneMap_;
};

#endif
