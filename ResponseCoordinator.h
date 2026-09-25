#ifndef RESPONSECOORDINATOR_H
#define RESPONSECOORDINATOR_H

#include "ResponseMediator.h"

class SecurityTeam;
class MedicalResponder;
class FacilitiesStaff;
class CommunicationService;

class ResponseCoordinator : public ResponseMediator {
private:
    SecurityTeam* security_;                 // non-owning
    MedicalResponder* medical_;              // non-owning
    FacilitiesStaff* facilities_;            // non-owning
    CommunicationService* communications_;   // non-owning

public:
    ResponseCoordinator();

    void setSecurity(SecurityTeam* security);
    void setMedical(MedicalResponder* medical);
    void setFacilities(FacilitiesStaff* facilities);
    void setCommunications(CommunicationService* communications);

    void notify(ResponseColleague* sender,
                const std::string& event,
                Incident& incident);
};

#endif
