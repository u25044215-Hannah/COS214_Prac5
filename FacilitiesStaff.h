#ifndef FACILITIESSTAFF_H
#define FACILITIESSTAFF_H

#include "ResponseColleague.h"
#include <string>

class Incident;

class FacilitiesStaff : public ResponseColleague {
private:
    std::string restrictedArea_;

public:
    explicit FacilitiesStaff(const std::string& name);

    void restrictArea(Incident& incident, const std::string& area);
    void cancelRestriction(Incident& incident);

    void unlockEmergencyRoute(Incident& incident);
    void restoreNormalAccess(Incident& incident);
    void acknowledgeAlert(Incident& incident);
};

#endif
