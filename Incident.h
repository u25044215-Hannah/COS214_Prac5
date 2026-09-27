#ifndef INCIDENT_H
#define INCIDENT_H

#include "IncidentState.h"
#include <string>
#include <vector>

class IncidentObserver;

/*incident holds its cur lifecycle, state, when state changes it notifies observers
 */

class Incident
{
public:
    Incident(const std::string &id, const std::string &location, const std::string &description);
    void attach(IncidentObserver *obs);
    void detach(IncidentObserver *obs);

    // lifecucle transitions
    void dispatch();
    void contain();
    void resolve();

    // called by IncidentState implementations only
    void setState(IncidentState &next);

    const std::string &id() const { return id_; }
    const std::string &location() const { return location_; }
    const std::string &description() const { return description_; }
    std::string stateName() const { return state_->name(); }

private:
    void notify(const std::string &oldState, const std::string &newState);

    std::string id_;
    std::string location_;
    std::string description_;
    IncidentState *state_;                      // non owning
    std::vector<IncidentObserver *> observers_; // non-owning
};

#endif
