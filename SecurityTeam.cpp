#include "SecurityTeam.h"
#include "Incident.h"
#include "ResponseMediator.h"
#include <iostream>

SecurityTeam::SecurityTeam(const std::string& name)
    : ResponseUnit(name), activeIncidentId_(-1) {}

void SecurityTeam::dispatch(Incident& incident) {
    activeIncidentId_ = incident.getId();
    std::cout << "[Receiver:SecurityTeam] " << name_ << " dispatched to incident #"
              << incident.getId() << " at " << incident.getLocation() << std::endl;

    if (mediator_) {
        mediator_->notify(this, "security_dispatched", incident);
    }
}

void SecurityTeam::standDown(Incident& incident) {
    std::cout << "[Receiver:SecurityTeam] " << name_ << " stood down from incident #"
              << incident.getId() << std::endl;
    if (activeIncidentId_ == incident.getId()) activeIncidentId_ = -1;
}

void SecurityTeam::secureSceneForMedical(Incident& incident) {
    std::cout << "[SecurityTeam] Securing scene and route for medical response at "
              << incident.getLocation() << std::endl;
}

void SecurityTeam::monitorRestrictedArea(Incident& incident) {
    std::cout << "[SecurityTeam] Monitoring restricted area for incident #"
              << incident.getId() << std::endl;
}

void SecurityTeam::acknowledgeAlert(Incident& incident) {
    std::cout << "[SecurityTeam] Emergency alert acknowledged for incident #"
              << incident.getId() << std::endl;
}
