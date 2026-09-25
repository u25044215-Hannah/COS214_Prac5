#ifndef INCIDENTREGISTRY_H
#define INCIDENTREGISTRY_H

#include <map>
#include <memory>
#include <string>
#include "Incident.h"

class IncidentRegistry {
private:
    std::map<int, std::unique_ptr<Incident> > incidents_;
    int nextId_;

    bool validTransition(IncidentStatus from, IncidentStatus to) const;

public:
    IncidentRegistry();

    Incident& registerIncident(const std::string& location, const std::string& description);
    Incident* findIncident(int id);
    const Incident* findIncident(int id) const;

    bool updateStatus(int id, IncidentStatus newStatus);
    bool restoreStatusForUndo(int id, IncidentStatus previousStatus);
};

#endif
