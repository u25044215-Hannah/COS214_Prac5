#include "IncidentBoard.h"
#include "Incident.h"
#include <iostream>

IncidentBoard::IncidentBoard(AuditLog &audit) : audit_(audit) {}

void IncidentBoard::onIncidentStateChanged(Incident &incident, const std::string &oldState, const std::string &newState)
{
    std::cout << "  [Board] " << incident.id() << " (" << incident.location() << "): " << oldState << " -> " << newState << "\n";
    audit_.record("STATE " + incident.id() + " " + oldState + " -> " + newState);
}
