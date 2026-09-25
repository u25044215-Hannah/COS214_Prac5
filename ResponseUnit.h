#ifndef RESPONSEUNIT_H
#define RESPONSEUNIT_H

#include "ResponseColleague.h"

class Incident;

class ResponseUnit : public ResponseColleague {
public:
    explicit ResponseUnit(const std::string& name) : ResponseColleague(name) {}
    virtual ~ResponseUnit() {}

    virtual void dispatch(Incident& incident) = 0;
    virtual void standDown(Incident& incident) = 0;
};

#endif
