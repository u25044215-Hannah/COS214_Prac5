#include "IncidentRegistry.h"
#include <iostream>

IncidentRegistry::IncidentRegistry() : nextId_(1) {}

Incident& IncidentRegistry::registerIncident(const std::string& location,
                                             const std::string& description) {
    const int id = nextId_++;
    std::unique_ptr<Incident> incident(new Incident(id, location, description));
    Incident& ref = *incident;
    incidents_[id] = std::move(incident);

    std::cout << "[IncidentRegistry] Registered incident #" << id
              << " at " << location << ": " << description << std::endl;
    return ref;
}

Incident* IncidentRegistry::findIncident(int id) {
    std::map<int, std::unique_ptr<Incident> >::iterator it = incidents_.find(id);
    return it == incidents_.end() ? 0 : it->second.get();
}

const Incident* IncidentRegistry::findIncident(int id) const {
    std::map<int, std::unique_ptr<Incident> >::const_iterator it = incidents_.find(id);
    return it == incidents_.end() ? 0 : it->second.get();
}

bool IncidentRegistry::validTransition(IncidentStatus from, IncidentStatus to) const {
    if (from == to) return true;

    if (from == IncidentStatus::REPORTED) {
        return to == IncidentStatus::ACTIVE || to == IncidentStatus::CANCELLED;
    }
    if (from == IncidentStatus::ACTIVE) {
        return to == IncidentStatus::CONTAINED || to == IncidentStatus::CANCELLED;
    }
    if (from == IncidentStatus::CONTAINED) {
        return to == IncidentStatus::RESOLVED;
    }
    return false;
}

bool IncidentRegistry::updateStatus(int id, IncidentStatus newStatus) {
    Incident* incident = findIncident(id);
    if (!incident) {
        std::cerr << "[IncidentRegistry] ERROR: incident #" << id << " does not exist." << std::endl;
        return false;
    }

    const IncidentStatus oldStatus = incident->getStatus();
    if (!validTransition(oldStatus, newStatus)) {
        std::cerr << "[IncidentRegistry] ERROR: invalid transition for incident #" << id
                  << " from " << incidentStatusToString(oldStatus)
                  << " to " << incidentStatusToString(newStatus) << "." << std::endl;
        return false;
    }

    incident->setStatus(newStatus);
    std::cout << "[IncidentRegistry] Incident #" << id << " status: "
              << incidentStatusToString(oldStatus) << " -> "
              << incidentStatusToString(newStatus) << std::endl;
    return true;
}

bool IncidentRegistry::restoreStatusForUndo(int id, IncidentStatus previousStatus) {
    Incident* incident = findIncident(id);
    if (!incident) {
        std::cerr << "[IncidentRegistry] ERROR: cannot restore missing incident #" << id << "." << std::endl;
        return false;
    }

    const IncidentStatus current = incident->getStatus();
    incident->setStatus(previousStatus);
    std::cout << "[IncidentRegistry] Undo restored incident #" << id << " status: "
              << incidentStatusToString(current) << " -> "
              << incidentStatusToString(previousStatus) << std::endl;
    return true;
}
