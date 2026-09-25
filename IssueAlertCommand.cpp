#include "IssueAlertCommand.h"
#include "IncidentRegistry.h"
#include "CommunicationServices.h"
#include "Incident.h"
#include <iostream>

IssueAlertCommand::IssueAlertCommand(IncidentRegistry& registry,
                                     CommunicationService& receiver,
                                     int incidentId,
                                     const std::string& message)
    : registry_(registry), receiver_(receiver), incidentId_(incidentId),
      message_(message), executed_(false) {}

void IssueAlertCommand::execute() {
    Incident* incident = registry_.findIncident(incidentId_);
    if (!incident) {
        std::cerr << "[IssueAlertCommand] ERROR: incident #" << incidentId_
                  << " does not exist." << std::endl;
        return;
    }

    if (incident->getStatus() == IncidentStatus::RESOLVED ||
        incident->getStatus() == IncidentStatus::CANCELLED) {
        std::cerr << "[IssueAlertCommand] ERROR: cannot alert for incident #"
                  << incidentId_ << " while status is "
                  << incidentStatusToString(incident->getStatus()) << "." << std::endl;
        return;
    }

    receiver_.issueCampusAlert(*incident, message_);
    executed_ = true;
}

void IssueAlertCommand::undo() {
    if (!executed_) {
        std::cout << "[IssueAlertCommand] Nothing to undo." << std::endl;
        return;
    }

    Incident* incident = registry_.findIncident(incidentId_);
    if (!incident) return;

    receiver_.clearAlert(*incident);
    executed_ = false;
}

std::string IssueAlertCommand::description() const {
    return "Issue campus alert for incident #" + std::to_string(incidentId_);
}
