#include "Incident.h"

Incident::Incident(int id, const std::string& location, const std::string& description)
    : id_(id), location_(location), description_(description), status_(IncidentStatus::REPORTED) {}

int Incident::getId() const { return id_; }
const std::string& Incident::getLocation() const { return location_; }
const std::string& Incident::getDescription() const { return description_; }
IncidentStatus Incident::getStatus() const { return status_; }
void Incident::setStatus(IncidentStatus status) { status_ = status; }

std::string incidentStatusToString(IncidentStatus status) {
    switch (status) {
        case IncidentStatus::REPORTED: return "REPORTED";
        case IncidentStatus::ACTIVE: return "ACTIVE";
        case IncidentStatus::CONTAINED: return "CONTAINED";
        case IncidentStatus::RESOLVED: return "RESOLVED";
        case IncidentStatus::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}
