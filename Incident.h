#ifndef INCIDENT_H
#define INCIDENT_H

#include "IncidentState.h"
#include <string>
#include <vector>

class IncidentObserver;

enum class IncidentStatus
{
    REPORTED,
    ACTIVE,
    CONTAINED,
    RESOLVED,
    CANCELLED
};

std::string incidentStatusToString(IncidentStatus status);

class Incident
{
public:
    // Constructor used by IncidentRegistry / Command / Mediator code.
    Incident(int id, const std::string &location, const std::string &description);

    // Original State + Observer constructor retained for compatibility.
    Incident(const std::string &id, const std::string &location,
             const std::string &description);

    // Convenience constructor used by the integrated demo.
    explicit Incident(const std::string &description);

    void attach(IncidentObserver *obs);
    void detach(IncidentObserver *obs);

    // State-pattern lifecycle operations.
    void dispatch();
    void contain();
    void resolve();
    void progress();
    void setState(IncidentState &next);

    // API required by Registry, Commands and ResponseUnits.
    int getId() const { return numericId_; }
    int getID() const { return numericId_; } // compatibility spelling
    const std::string &getLocation() const { return location_; }
    const std::string &getDescription() const { return description_; }
    IncidentStatus getStatus() const { return status_; }
    void setStatus(IncidentStatus status);

    // API retained by the original State + Observer implementation.
    const std::string &id() const { return id_; }
    const std::string &location() const { return location_; }
    const std::string &description() const { return description_; }
    std::string stateName() const { return state_->name(); }

    std::string describe() const;

private:
    void notify(const std::string &oldState, const std::string &newState);
    void syncStatusFromState();

    int numericId_;
    std::string id_;
    std::string location_;
    std::string description_;
    IncidentState *state_;                      // non-owning singleton State
    IncidentStatus status_;
    std::vector<IncidentObserver *> observers_; // non-owning
};

#endif
