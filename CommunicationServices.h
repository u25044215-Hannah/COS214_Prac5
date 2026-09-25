#ifndef COMMUNICATIONSERVICE_H
#define COMMUNICATIONSERVICE_H

#include "ResponseColleague.h"
#include <string>

class Incident;

class CommunicationService : public ResponseColleague {
private:
    bool alertActive_;

public:
    explicit CommunicationService(const std::string& name);

    void issueCampusAlert(Incident& incident, const std::string& message);
    void clearAlert(Incident& incident);
    void issueInternalNotice(Incident& incident, const std::string& message);
};

#endif
