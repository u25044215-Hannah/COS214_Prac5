#include "DispatchUnitCommand.h"
#include "IncidentRegistry.h"
#include "ResponseUnit.h"
#include <iostream>

DispatchUnitCommand::DispatchUnitCommand(IncidentRegistry& registry,
                                         ResponseUnit& receiver,
                                         int incidentId)
    : registry_(registry), receiver_(receiver), incidentId_(incidentId),
      executed_(false), previousStatus_(IncidentStatus::REPORTED) {}

void DispatchUnitCommand::execute() {
    Incident* incident = registry_.findIncident(incidentId_);
    if (!incident) {
        std::cerr << "[DispatchUnitCommand] ERROR: incident #" << incidentId_
                  << " does not exist." << std::endl;
        return;
    }

    if (incident->getStatus() == IncidentStatus::RESOLVED ||
        incident->getStatus() == IncidentStatus::CANCELLED) {
        std::cerr << "[DispatchUnitCommand] ERROR: cannot dispatch to incident #"
                  << incidentId_ << " while status is "
                  << incidentStatusToString(incident->getStatus()) << "." << std::endl;
        return;
    }

    previousStatus_ = incident->getStatus();
    if (incident->getStatus() == IncidentStatus::REPORTED) {
        if (!registry_.updateStatus(incidentId_, IncidentStatus::ACTIVE)) return;
    }

    receiver_.dispatch(*incident);
    executed_ = true;
}

void DispatchUnitCommand::undo() {
    if (!executed_) {
        std::cout << "[DispatchUnitCommand] Nothing to undo." << std::endl;
        return;
    }

    Incident* incident = registry_.findIncident(incidentId_);
    if (!incident) return;

    receiver_.standDown(*incident);
    registry_.restoreStatusForUndo(incidentId_, previousStatus_);
    executed_ = false;
}

std::string DispatchUnitCommand::description() const {
    return "Dispatch response unit to incident #" + std::to_string(incidentId_);
}
