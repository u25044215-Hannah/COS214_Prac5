#ifndef INCIDENTBOARD_H
#define INCIDENTBOARD_H

#include "AuditLog.h"
#include "IncidentObserver.h"

class IncidentBoard : public IncidentObserver
{
public:
    explicit IncidentBoard(AuditLog &audit);
    void onIncidentStateChanged(Incident &incident, const std::string &oldState, const std::string &newState);

private:
    AuditLog &audit_;
};

#endif
