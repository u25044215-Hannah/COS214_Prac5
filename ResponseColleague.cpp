#ifndef RESPONSECOLLEAGUE_H
#define RESPONSECOLLEAGUE_H

#include <string>

class ResponseMediator;

class ResponseColleague {
protected:
    ResponseMediator* mediator_; // non-owning
    std::string name_;

public:
    explicit ResponseColleague(const std::string& name);
    virtual ~ResponseColleague() {}

    void setMediator(ResponseMediator* mediator);
    const std::string& getName() const;
};

#endif
