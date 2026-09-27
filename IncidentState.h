#ifndef INCIDENTSTATE_H
#define INCIDENTSTATE_H

#include <stdexcept>
#include <string>

class Incident;

// failure is caught by Incident and reported to observers, so that the system can react
class IncidentTransitionError : public std::runtime_error
{
public:
    explicit IncidentTransitionError(const std::string &msg) : std::runtime_error(msg) {}
};
// defines what lifecycle is allowed
// stateless
class IncidentState
{
public:
    virtual ~IncidentState() {}
    virtual void dispatch(Incident &incident) = 0;
    virtual void contain(Incident &incident) = 0;
    virtual void resolve(Incident &incident) = 0;
    virtual std::string name() const = 0;

protected:
    // helper for error
    void reject(Incident &incident, const std::string &attempted) const;
};

class ReportedState : public IncidentState
{
public:
    static IncidentState &instance();
    void dispatch(Incident &incident);
    void contain(Incident &incident);
    void resolve(Incident &incident);
    std::string name() const { return "REPORTED"; }
};

class DispatchedState : public IncidentState
{
public:
    static IncidentState &instance();
    void dispatch(Incident &incident);
    void contain(Incident &incident);
    void resolve(Incident &incident);
    std::string name() const { return "DISPATCHED"; }
};

class ContainedState : public IncidentState
{
public:
    static IncidentState &instance();
    void dispatch(Incident &incident);
    void contain(Incident &incident);
    void resolve(Incident &incident);
    std::string name() const { return "CONTAINED"; }
};

class ResolvedState : public IncidentState
{
public:
    static IncidentState &instance();
    void dispatch(Incident &incident);
    void contain(Incident &incident);
    void resolve(Incident &incident);
    std::string name() const { return "RESOLVED"; }
};

#endif
