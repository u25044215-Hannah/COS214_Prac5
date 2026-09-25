#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

enum class IncidentStatus {
    REPORTED,
    ACTIVE,
    CONTAINED,
    RESOLVED,
    CANCELLED
};

std::string incidentStatusToString(IncidentStatus status);

class Incident {
private:
    int id_;
    std::string location_;
    std::string description_;
    IncidentStatus status_;

public:
    Incident(int id, const std::string& location, const std::string& description);

    int getId() const;
    const std::string& getLocation() const;
    const std::string& getDescription() const;
    IncidentStatus getStatus() const;

    void setStatus(IncidentStatus status);
};

#endif
