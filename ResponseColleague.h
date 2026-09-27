#ifndef RESPONSECOLLEAGUE_H
#define RESPONSECOLLEAGUE_H

#include <string>

class ResponseMediator;
class Incident;

class ResponseColleague
{
public:
    explicit ResponseColleague(const std::string& name);
    virtual ~ResponseColleague();

    void setMediator(ResponseMediator* mediator);

    const std::string& getName() const;

    virtual void receive(
        const std::string& event,
        Incident& incident
    ) = 0;

protected:
    ResponseMediator* mediator_;
    std::string name_;
};

#endif