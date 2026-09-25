#include "ResponseCoordinator.h"
#include "SecurityTeam.h"
#include "MedicalResponder.h"
#include "FacilitiesStaff.h"
#include "CommunicationServices.h"
#include "ResponseColleague.h"
#include "Incident.h"
#include <iostream>

ResponseCoordinator::ResponseCoordinator()
    : security_(0), medical_(0), facilities_(0), communications_(0) {}

void ResponseCoordinator::setSecurity(SecurityTeam* security) { security_ = security; }
void ResponseCoordinator::setMedical(MedicalResponder* medical) { medical_ = medical; }
void ResponseCoordinator::setFacilities(FacilitiesStaff* facilities) { facilities_ = facilities; }
void ResponseCoordinator::setCommunications(CommunicationService* communications) { communications_ = communications; }

void ResponseCoordinator::notify(ResponseColleague* sender,
                                 const std::string& event,
                                 Incident& incident) {
    std::cout << "[Mediator:ResponseCoordinator] Event '" << event << "' received from "
              << (sender ? sender->getName() : std::string("unknown"))
              << " for incident #" << incident.getId() << std::endl;

    if (event == "security_dispatched") {
        if (facilities_) facilities_->unlockEmergencyRoute(incident);
        if (medical_) medical_->prepareStandby(incident);
        if (communications_) communications_->issueInternalNotice(
            incident, "Security dispatched; emergency route opened and medical placed on standby.");
        return;
    }

    if (event == "medical_dispatched") {
        if (security_) security_->secureSceneForMedical(incident);
        if (facilities_) facilities_->unlockEmergencyRoute(incident);
        if (communications_) communications_->issueInternalNotice(
            incident, "Medical response dispatched; security and facilities coordinated.");
        return;
    }

    if (event == "area_restricted") {
        if (security_) security_->monitorRestrictedArea(incident);
        if (communications_) communications_->issueInternalNotice(
            incident, "Facilities changed access controls; security monitoring requested.");
        return;
    }

    if (event == "campus_alert_issued") {
        if (security_) security_->acknowledgeAlert(incident);
        if (medical_) medical_->acknowledgeAlert(incident);
        if (facilities_) facilities_->acknowledgeAlert(incident);
        return;
    }

    std::cout << "[Mediator:ResponseCoordinator] No coordination rule for event '"
              << event << "'." << std::endl;
}
