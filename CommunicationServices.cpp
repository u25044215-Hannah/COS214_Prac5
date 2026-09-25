#include "CommunicationServices.h"
#include "Incident.h"
#include "ResponseMediator.h"
#include <iostream>

CommunicationService::CommunicationService(const std::string& name)
    : ResponseColleague(name), alertActive_(false) {}

void CommunicationService::issueCampusAlert(Incident& incident,
                                             const std::string& message) {
    alertActive_ = true;
    std::cout << "[Receiver:CommunicationService] CAMPUS ALERT for incident #"
              << incident.getId() << ": " << message << std::endl;

    if (mediator_) {
        mediator_->notify(this, "campus_alert_issued", incident);
    }
}

void CommunicationService::clearAlert(Incident& incident) {
    if (!alertActive_) {
        std::cout << "[CommunicationService] No active alert to clear for incident #"
                  << incident.getId() << std::endl;
        return;
    }

    alertActive_ = false;
    std::cout << "[CommunicationService] Cleared campus alert for incident #"
              << incident.getId() << std::endl;
}

void CommunicationService::issueInternalNotice(Incident& incident,
                                                const std::string& message) {
    std::cout << "[CommunicationService] Internal notice for incident #"
              << incident.getId() << ": " << message << std::endl;
}
