#include "IncidentState.h"
#include "Incident.h"
#include <sstream>

void IncidentState::reject(Incident &incident, const std::string &attempted) const
{
    std::ostringstream os;
    os << "incident " << incident.id() << ": cannot " << attempted << "() while in state " << name();
    throw IncidentTransitionError(os.str());
}
// reported
IncidentState &ReportedState::instance()
{
    static ReportedState s;
    return s;
}
void ReportedState::dispatch(Incident &incident) { incident.setState(DispatchedState::instance()); }
void ReportedState::contain(Incident &incident) { reject(incident, "contain"); }
void ReportedState::resolve(Incident &incident) { reject(incident, "resolve"); }

// dispatched
IncidentState &DispatchedState::instance()
{
    static DispatchedState s;
    return s;
}
void DispatchedState::dispatch(Incident &incident) { incident.setState(DispatchedState::instance()); }
void DispatchedState::contain(Incident &incident) { incident.setState(ContainedState::instance()); }
void DispatchedState::resolve(Incident &incident) { reject(incident, "resolve"); }

// contained
IncidentState &ContainedState::instance()
{
    static ContainedState s;
    return s;
}
void ContainedState::dispatch(Incident &incident) { reject(incident, "dispatch"); }
void ContainedState::contain(Incident &incident) { reject(incident, "contain"); }
void ContainedState::resolve(Incident &incident) { incident.setState(ResolvedState::instance()); }

// resolved
IncidentState &ResolvedState::instance()
{
    static ResolvedState s;
    return s;
}
void ResolvedState::dispatch(Incident &incident) { reject(incident, "dispatch"); }
void ResolvedState::contain(Incident &incident) { reject(incident, "contain"); }
void ResolvedState::resolve(Incident &incident) { reject(incident, "resolve"); }
