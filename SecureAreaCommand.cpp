#include "SecureAreaCommand.h"
#include "IncidentRegistry.h"
#include "FacilitiesStaff.h"
#include "Incident.h"
#include <iostream>

SecureAreaCommand::SecureAreaCommand(IncidentRegistry& registry,
                                     FacilitiesStaff& receiver,
                                     int incidentId,
                                     const std::string& area)
    : registry_(registry), receiver_(receiver), incidentId_(incidentId),
      area_(area), executed_(false) {}

void SecureAreaCommand::execute() {
    Incident* incident = registry_.findIncident(incidentId_);
    if (!incident) {
        std::cerr << "[SecureAreaCommand] ERROR: incident #" << incidentId_
                  << " does not exist." << std::endl;
        return;
    }

    if (incident->getStatus() == IncidentStatus::RESOLVED ||
        incident->getStatus() == IncidentStatus::CANCELLED) {
        std::cerr << "[SecureAreaCommand] ERROR: cannot restrict area for incident #"
                  << incidentId_ << " while status is "
                  << incidentStatusToString(incident->getStatus()) << "." << std::endl;
        return;
    }

    receiver_.restrictArea(*incident, area_);
    executed_ = true;
}

void SecureAreaCommand::undo() {
    if (!executed_) {
        std::cout << "[SecureAreaCommand] Nothing to undo." << std::endl;
        return;
    }

    Incident* incident = registry_.findIncident(incidentId_);
    if (!incident) return;

    receiver_.cancelRestriction(*incident);
    executed_ = false;
}

std::string SecureAreaCommand::description() const {
    return "Restrict " + area_ + " for incident #" + std::to_string(incidentId_);
}
