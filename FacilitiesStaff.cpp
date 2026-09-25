#include "FacilitiesStaff.h"
#include "Incident.h"
#include "ResponseMediator.h"
#include <iostream>

FacilitiesStaff::FacilitiesStaff(const std::string& name)
    : ResponseColleague(name), restrictedArea_("") {}

void FacilitiesStaff::restrictArea(Incident& incident, const std::string& area) {
    restrictedArea_ = area;
    std::cout << "[Receiver:FacilitiesStaff] Restricted access to " << area
              << " for incident #" << incident.getId() << std::endl;

    if (mediator_) {
        mediator_->notify(this, "area_restricted", incident);
    }
}

void FacilitiesStaff::cancelRestriction(Incident& incident) {
    if (restrictedArea_.empty()) {
        std::cout << "[FacilitiesStaff] No active restriction to cancel for incident #"
                  << incident.getId() << std::endl;
        return;
    }

    std::cout << "[FacilitiesStaff] Cancelled restriction on " << restrictedArea_
              << " for incident #" << incident.getId() << std::endl;
    restrictedArea_.clear();
}

void FacilitiesStaff::unlockEmergencyRoute(Incident& incident) {
    std::cout << "[FacilitiesStaff] Emergency route unlocked for incident #"
              << incident.getId() << std::endl;
}

void FacilitiesStaff::restoreNormalAccess(Incident& incident) {
    std::cout << "[FacilitiesStaff] Normal access restored after incident #"
              << incident.getId() << std::endl;
}

void FacilitiesStaff::acknowledgeAlert(Incident& incident) {
    std::cout << "[FacilitiesStaff] Emergency alert acknowledged for incident #"
              << incident.getId() << std::endl;
}
