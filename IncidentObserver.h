#ifndef INCIDENTOBSERVER_H
#define INCIDENTOBSERVER_H

#include <string>

class Incident;
class IncidentObserver
{
public:
    virtual ~IncidentObserver() {}
    virtual void onIncidentStateChanged(Incident &incident, const std::string &oldState, const std::string &newState) = 0;
};

#endif
