#ifndef SECUREAREACOMMAND_H
#define SECUREAREACOMMAND_H

#include "Command.h"
#include <string>

class IncidentRegistry;
class FacilitiesStaff;

class SecureAreaCommand : public Command {
private:
    IncidentRegistry& registry_;
    FacilitiesStaff& receiver_;
    int incidentId_;
    std::string area_;
    bool executed_;

public:
    SecureAreaCommand(IncidentRegistry& registry,
                      FacilitiesStaff& receiver,
                      int incidentId,
                      const std::string& area);

    void execute();
    void undo();
    std::string description() const;
};

#endif
