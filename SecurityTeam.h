#ifndef SECURITYTEAM_H
#define SECURITYTEAM_H

#include "ResponseUnit.h"

class SecurityTeam : public ResponseUnit {
private:
    int activeIncidentId_;

public:
    explicit SecurityTeam(const std::string& name);

    void dispatch(Incident& incident);
    void standDown(Incident& incident);

    void secureSceneForMedical(Incident& incident);
    void monitorRestrictedArea(Incident& incident);
    void acknowledgeAlert(Incident& incident);
};

#endif
