#include <iostream>
#include <memory>

#include "AuditLog.h"
#include "Incident.h"
#include "IncidentBoard.h"
#include "IncidentRegistry.h"
#include "ResponseCoordinator.h"
#include "SecurityTeam.h"
#include "MedicalResponder.h"
#include "FacilitiesStaff.h"
#include "CommunicationServices.h"
#include "OperatorConsole.h"
#include "DispatchUnitCommand.h"
#include "SecureAreaCommand.h"
#include "IssueAlertCommand.h"
#include "CancelActionCommand.h"
#include "LegacyDoorPanel.h"
#include "LegacyPanelAdapter.h"
#include "BuildingAccessService.h"
#include "NotificationService.h"
#include "LockdownFacade.h"

int main() {
    std::cout << "========================================\n"
              << " CAMPUSGUARD INTEGRATED TEST\n"
              << "========================================\n";

    AuditLog audit;
    IncidentBoard board(audit);

    std::cout << "\n--- 1. STATE + OBSERVER ---\n";
    Incident lifecycle(900, "Engineering Lab", "Smoke detected");
    lifecycle.attach(&board);
    lifecycle.dispatch();
    lifecycle.contain();
    lifecycle.resolve();
    std::cout << "Final lifecycle: " << lifecycle.describe() << "\n";

    std::cout << "\n--- 2. MEDIATOR + RESPONSE COLLEAGUES ---\n";
    ResponseCoordinator coordinator;
    SecurityTeam security("Campus Security");
    MedicalResponder medical("Medical Response");
    FacilitiesStaff facilities("Facilities");
    CommunicationService communications("Communications");

    security.setMediator(&coordinator);
    medical.setMediator(&coordinator);
    facilities.setMediator(&coordinator);
    communications.setMediator(&coordinator);
    coordinator.setSecurity(&security);
    coordinator.setMedical(&medical);
    coordinator.setFacilities(&facilities);
    coordinator.setCommunications(&communications);

    std::cout << "\n--- 3. REGISTRY + COMMAND + INVOKER ---\n";
    IncidentRegistry registry;
    Incident& fire = registry.registerIncident("Engineering Building", "Laboratory fire");
    fire.attach(&board);
    const int fireId = fire.getId();

    OperatorConsole console;
    console.execute(std::unique_ptr<Command>(new DispatchUnitCommand(registry, security, fireId)));
    console.execute(std::unique_ptr<Command>(new SecureAreaCommand(registry, facilities, fireId, "Engineering Lab")));
    console.execute(std::unique_ptr<Command>(new IssueAlertCommand(registry, communications, fireId,
                    "Evacuate Engineering Building immediately.")));

    std::cout << "Commands stored by invoker: " << console.commandCount() << "\n";

    std::cout << "\n--- 4. COMMAND UNDO + CANCEL COMMAND ---\n";
    Command* last = console.lastCommand();
    if (last) {
        CancelActionCommand cancel(*last);
        cancel.execute();
        cancel.undo();
    }

    std::cout << "\n--- 5. ADAPTER + BUILDING ACCESS SERVICE ---\n";
    LegacyDoorPanel legacyPanel;
    std::unique_ptr<LegacyPanelAdapter> adapter(new LegacyPanelAdapter(legacyPanel));
    LegacyPanelAdapter* adapterView = adapter.get();
    adapterView->mapArea("ENG-ENTRANCE", 101);
    adapterView->mapArea("ENG-LAB", 102);
    adapterView->mapArea("ENG-OFFICE", 103);

    std::unique_ptr<AccessController> controller(adapter.release());
    BuildingAccessService access(std::move(controller));
    access.addArea("ENG-ENTRANCE", "ENG", "Engineering Entrance", AREA_ENTRANCE);
    access.addArea("ENG-LAB", "ENG", "Engineering Laboratory", AREA_INTERIOR);
    access.addArea("ENG-OFFICE", "ENG", "Engineering Office", AREA_INTERIOR);

    access.lockArea("ENG-ENTRANCE");
    access.restrictArea("ENG-LAB");
    access.unlockArea("ENG-ENTRANCE");

    std::cout << "\n--- 6. FACADE: COMPLETE LOCKDOWN ---\n";
    NotificationService notifier;
    LockdownFacade facade(access, notifier, audit);
    LockdownReport report = facade.initiateLockdown("ENG", "INC-" + std::to_string(fireId));
    std::cout << "Lockdown secured=" << report.secured
              << " failed=" << report.failed.size() << "\n";

    std::cout << "\n--- 7. FACADE FAILURE PATH ---\n";
    legacyPanel.setZoneOffline(102, true);
    LockdownReport partial = facade.liftLockdown("ENG", "INC-" + std::to_string(fireId));
    std::cout << "Release secured=" << partial.secured
              << " failed=" << partial.failed.size() << "\n";
    legacyPanel.setZoneOffline(102, false);

    std::cout << "\n--- 8. INVALID INPUT / ERROR HANDLING ---\n";
    console.execute(std::unique_ptr<Command>(new DispatchUnitCommand(registry, medical, 9999)));
    LockdownReport unknown = facade.initiateLockdown("UNKNOWN", "INC-X");
    std::cout << "Unknown-building failures=" << unknown.failed.size() << "\n";

    std::cout << "\n--- SUMMARY ---\n";
    std::cout << "Audit entries: " << audit.entries().size() << "\n";
    std::cout << "Notifications sent: " << notifier.sentCount() << "\n";
    std::cout << "All integrated tests completed.\n";
    return 0;
}
