#include "Incident.h"
#include "IncidentObserver.h"
#include <algorithm>

Incident::Incident(const std::string &id, const std::string &location, const std::string &description) : id_(id), location_(location), description_(description), state_(&ReportedState::instance()) {}

void Incident::attach(IncidentObserver *obs)
{
    if (obs)
        observers_.push_back(obs);
}

void Incident::detach(IncidentObserver *obs)
{
    observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
}

void Incident::setState(IncidentState &next) { state_ = &next; }

void Incident::dispatch()
{
    std::string before = state_->name();
    state_->dispatch(*this); // posible error
    std::string after = state_->name();
    if (before != after)
        notify(before, after);
}

void Incident::contain()
{
    std::string before = state_->name();
    state_->contain(*this);
    std::string after = state_->name();
    if (before != after)
        notify(before, after);
}

void Incident::resolve()
{
    std::string before = state_->name();
    state_->resolve(*this);
    std::string after = state_->name();
    if (before != after)
        notify(before, after);
}

void Incident::notify(const std::string &oldState, const std::string &newState)
{
    for (size_t i = 0; i < observers_.size(); ++i)
    {
        observers_[i]->onIncidentStateChanged(*this, oldState, newState);
    }
}
