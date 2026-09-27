#include "Incident.h"
#include "IncidentObserver.h"
#include <algorithm>
#include <iostream>

Incident::Incident(const std::string &id,
                   const std::string &location,
                   const std::string &description)
    : id_(id),
      location_(location),
      description_(description),
      state_(&ReportedState::instance())
{
}

Incident::Incident(const std::string &description)
    : id_("INCIDENT"),
      location_("Campus"),
      description_(description),
      state_(&ReportedState::instance())
{
}

void Incident::attach(IncidentObserver *obs)
{
    if (obs && std::find(observers_.begin(), observers_.end(), obs) == observers_.end())
    {
        observers_.push_back(obs);
    }
}

void Incident::detach(IncidentObserver *obs)
{
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), obs),
        observers_.end());
}

void Incident::setState(IncidentState &next)
{
    state_ = &next;
}

void Incident::dispatch()
{
    std::string before = state_->name();
    state_->dispatch(*this);
    std::string after = state_->name();

    if (before != after)
    {
        notify(before, after);
    }
}

void Incident::contain()
{
    std::string before = state_->name();
    state_->contain(*this);
    std::string after = state_->name();

    if (before != after)
    {
        notify(before, after);
    }
}

void Incident::resolve()
{
    std::string before = state_->name();
    state_->resolve(*this);
    std::string after = state_->name();

    if (before != after)
    {
        notify(before, after);
    }
}

void Incident::progress()
{
    const std::string current = stateName();

    // Support the group's lifecycle naming as well as the names used by the
    // integrated demonstration. The actual transition remains delegated to
    // the existing State objects.
    if (current == "REPORTED" || current == "Reported" ||
        current == "NEW" || current == "New")
    {
        dispatch();
    }
    else if (current == "DISPATCHED" || current == "Dispatched" ||
             current == "ASSIGNED" || current == "Assigned")
    {
        contain();
    }
    else if (current == "CONTAINED" || current == "Contained" ||
             current == "MITIGATION" || current == "Mitigation")
    {
        resolve();
    }
    else
    {
        std::cout << "[Incident State] INVALID: incident is already "
                  << current << "." << std::endl;
    }
}

std::string Incident::describe() const
{
    return description_ + " [" + stateName() + "]";
}

void Incident::notify(const std::string &oldState, const std::string &newState)
{
    for (size_t i = 0; i < observers_.size(); ++i)
    {
        observers_[i]->onIncidentStateChanged(*this, oldState, newState);
    }
}