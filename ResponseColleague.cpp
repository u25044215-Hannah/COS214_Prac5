#include "ResponseColleague.h"
#include "ResponseMediator.h"

ResponseColleague::ResponseColleague(const std::string& name)
    : mediator_(nullptr),
      name_(name)
{
}

ResponseColleague::~ResponseColleague()
{
}

void ResponseColleague::setMediator(ResponseMediator* mediator)
{
    mediator_ = mediator;
}

const std::string& ResponseColleague::getName() const
{
    return name_;
}