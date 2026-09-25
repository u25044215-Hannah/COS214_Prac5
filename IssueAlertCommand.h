#ifndef ISSUEALERTCOMMAND_H
#define ISSUEALERTCOMMAND_H

#include "Command.h"
#include <string>

class IncidentRegistry;
class CommunicationService;

class IssueAlertCommand : public Command {
private:
    IncidentRegistry& registry_;
    CommunicationService& receiver_;
    int incidentId_;
    std::string message_;
    bool executed_;

public:
    IssueAlertCommand(IncidentRegistry& registry,
                      CommunicationService& receiver,
                      int incidentId,
                      const std::string& message);

    void execute();
    void undo();
    std::string description() const;
};

#endif
