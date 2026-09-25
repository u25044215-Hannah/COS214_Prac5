#ifndef RESPONSEMEDIATOR_H
#define RESPONSEMEDIATOR_H

#include <string>

class ResponseColleague;
class Incident;

class ResponseMediator {
public:
    virtual ~ResponseMediator() {}
    virtual void notify(ResponseColleague* sender,
                        const std::string& event,
                        Incident& incident) = 0;
};

#endif
