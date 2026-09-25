#ifndef MEDICALRESPONDER_H
#define MEDICALRESPONDER_H

#include "ResponseUnit.h"

class MedicalResponder : public ResponseUnit {
private:
    int activeIncidentId_;

public:
    explicit MedicalResponder(const std::string& name);

    void dispatch(Incident& incident);
    void standDown(Incident& incident);

    void prepareStandby(Incident& incident);
    void acknowledgeAlert(Incident& incident);
};

#endif
