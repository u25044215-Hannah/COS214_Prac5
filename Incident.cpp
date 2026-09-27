#include "Incident.h"
#include "IncidentObserver.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

std::string incidentStatusToString(IncidentStatus status)
{
    switch (status)
    {
    case IncidentStatus::REPORTED: return "REPORTED";
    case IncidentStatus::ACTIVE: return "ACTIVE";
    case IncidentStatus::CONTAINED: return "CONTAINED";
    case IncidentStatus::RESOLVED: return "RESOLVED";
    case IncidentStatus::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

Incident::Incident(int id, const std::string &location,
                   const std::string &description)
    : numericId_(id), id_(std::to_string(id)), location_(location),
      description_(description), state_(&ReportedState::instance()),
      status_(IncidentStatus::REPORTED)
{
}

Incident::Incident(const std::string &id, const std::string &location,
                   const std::string &description)
    : numericId_(std::atoi(id.c_str())), id_(id), location_(location),
      description_(description), state_(&ReportedState::instance()),
      status_(IncidentStatus::REPORTED)
{
}

Incident::Incident(const std::string &description)
    : numericId_(0), id_("INCIDENT"), location_("Campus"),
      description_(description), state_(&ReportedState::instance()),
      status_(IncidentStatus::REPORTED)
{
}

void Incident::attach(IncidentObserver *obs)
{
    if (obs && std::find(observers_.begin(), observers_.end(), obs) == observers_.end())
        observers_.push_back(obs);
}

void Incident::detach(IncidentObserver *obs)
{
    observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
}

void Incident::setState(IncidentState &next)
{
    state_ = &next;
    syncStatusFromState();
}

void Incident::syncStatusFromState()
{
    const std::string name = state_->name();
    if (name == "REPORTED") status_ = IncidentStatus::REPORTED;
    else if (name == "DISPATCHED") status_ = IncidentStatus::ACTIVE;
    else if (name == "CONTAINED") status_ = IncidentStatus::CONTAINED;
    else if (name == "RESOLVED") status_ = IncidentStatus::RESOLVED;
}

void Incident::setStatus(IncidentStatus status)
{
    status_ = status;

    // Keep the older enum-based Registry API and the newer State object in sync.
    switch (status)
    {
    case IncidentStatus::REPORTED: state_ = &ReportedState::instance(); break;
    case IncidentStatus::ACTIVE: state_ = &DispatchedState::instance(); break;
    case IncidentStatus::CONTAINED: state_ = &ContainedState::instance(); break;
    case IncidentStatus::RESOLVED: state_ = &ResolvedState::instance(); break;
    case IncidentStatus::CANCELLED:
        // There is no CancelledState in the group's State hierarchy.
        break;
    }
}

void Incident::dispatch()
{
    const std::string before = state_->name();
    state_->dispatch(*this);
    const std::string after = state_->name();
    if (before != after) notify(before, after);
}

void Incident::contain()
{
    const std::string before = state_->name();
    state_->contain(*this);
    const std::string after = state_->name();
    if (before != after) notify(before, after);
}

void Incident::resolve()
{
    const std::string before = state_->name();
    state_->resolve(*this);
    const std::string after = state_->name();
    if (before != after) notify(before, after);
}

void Incident::progress()
{
    if (status_ == IncidentStatus::CANCELLED)
    {
        std::cout << "[Incident State] INVALID: incident is CANCELLED." << std::endl;
        return;
    }

    const std::string current = stateName();
    if (current == "REPORTED") dispatch();
    else if (current == "DISPATCHED") contain();
    else if (current == "CONTAINED") resolve();
    else
        std::cout << "[Incident State] INVALID: incident is already "
                  << current << "." << std::endl;
}

std::string Incident::describe() const
{
    return description_ + " [" + incidentStatusToString(status_) + "]";
}

void Incident::notify(const std::string &oldState, const std::string &newState)
{
    for (size_t i = 0; i < observers_.size(); ++i)
        observers_[i]->onIncidentStateChanged(*this, oldState, newState);
}
