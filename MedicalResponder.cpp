#include "MedicalResponder.h"
#include "Incident.h"
#include "ResponseMediator.h"
#include <iostream>

MedicalResponder::MedicalResponder(const std::string& name)
    : ResponseUnit(name), activeIncidentId_(-1) {}

void MedicalResponder::dispatch(Incident& incident) {
    activeIncidentId_ = incident.getId();
    std::cout << "[Receiver:MedicalResponder] " << name_ << " dispatched to incident #"
              << incident.getId() << " at " << incident.getLocation() << std::endl;

    if (mediator_) {
        mediator_->notify(this, "medical_dispatched", incident);
    }
}

void MedicalResponder::standDown(Incident& incident) {
    std::cout << "[Receiver:MedicalResponder] " << name_ << " stood down from incident #"
              << incident.getId() << std::endl;
    if (activeIncidentId_ == incident.getId()) activeIncidentId_ = -1;
}

void MedicalResponder::prepareStandby(Incident& incident) {
    std::cout << "[MedicalResponder] Preparing medical standby for incident #"
              << incident.getId() << std::endl;
}

void MedicalResponder::acknowledgeAlert(Incident& incident) {
    std::cout << "[MedicalResponder] Emergency alert acknowledged for incident #"
              << incident.getId() << std::endl;
}
