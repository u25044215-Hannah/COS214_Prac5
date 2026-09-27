#ifndef INCIDENT_H
#define INCIDENT_H

#include "IncidentState.h"
#include <string>
#include <vector>

class IncidentObserver;

/*
 * Incident keeps the group's existing State + Observer implementation.
 * Compatibility helpers (one-argument constructor, progress(), describe())
 * are included so the integrated CampusGuard main/facade can also use it.
 */
class Incident
{
public:
    // Existing group constructor.
    Incident(const std::string &id,
             const std::string &location,
             const std::string &description);

    // Compatibility constructor used by the integrated CampusGuard scenarios.
    explicit Incident(const std::string &description);

    void attach(IncidentObserver *obs);
    void detach(IncidentObserver *obs);

    // Existing lifecycle transitions.
    void dispatch();
    void contain();
    void resolve();

    // Integrated-model helper: advances to the next lifecycle state.
    void progress();

    // Called by IncidentState implementations only.
    void setState(IncidentState &next);

    const std::string &id() const { return id_; }
    const std::string &location() const { return location_; }
    const std::string &description() const { return description_; }
    std::string stateName() const { return state_->name(); }

    // Integrated-model helper used when displaying an incident.
    std::string describe() const;

private:
    void notify(const std::string &oldState, const std::string &newState);

    std::string id_;
    std::string location_;
    std::string description_;
    IncidentState *state_;                      // non-owning singleton State
    std::vector<IncidentObserver *> observers_; // non-owning
};

#endif