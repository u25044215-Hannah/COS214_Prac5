#ifndef DISPATCHUNITCOMMAND_H
#define DISPATCHUNITCOMMAND_H

#include "Command.h"
#include "Incident.h"

class IncidentRegistry;
class ResponseUnit;

class DispatchUnitCommand : public Command {
private:
    IncidentRegistry& registry_;
    ResponseUnit& receiver_;
    int incidentId_;
    bool executed_;
    IncidentStatus previousStatus_;

public:
    DispatchUnitCommand(IncidentRegistry& registry,
                        ResponseUnit& receiver,
                        int incidentId);

    void execute();
    void undo();
    std::string description() const;
};

#endif
