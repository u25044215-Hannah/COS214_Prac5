#include <iostream>
#include <memory>
#include <string>

#include "AuditLog.h"
#include "Incident.h"
#include "IncidentBoard.h"
#include "IncidentRegistry.h"
#include "IncidentState.h"
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

static void banner(const std::string &title)
{
    std::cout << " " << title << "\n";
}

static void step(const std::string &title)
{
    std::cout << "\n- " << title << "\n";
}

static std::unique_ptr<AccessController> makeController(LegacyDoorPanel &panel)
{
    std::unique_ptr<LegacyPanelAdapter> adapter(new LegacyPanelAdapter(panel));
    adapter->mapArea("ENG-ENTRANCE", 101);
    adapter->mapArea("ENG-LAB", 102);
    adapter->mapArea("ENG-OFFICE", 103);
    adapter->mapArea("LIB-ENTRANCE", 201);
    adapter->mapArea("LIB-READING", 202);
    return std::unique_ptr<AccessController>(adapter.release());
}
struct CampusWorld
{
    AuditLog audit;
    IncidentBoard board;             // Observer (registered on every incident)
    ResponseCoordinator coordinator; // Mediator
    SecurityTeam security;           // Colleagues / Command receivers
    MedicalResponder medical;
    FacilitiesStaff facilities;
    CommunicationService communications;
    IncidentRegistry registry;    // owns Incidents
    OperatorConsole console;      // Command invoker (owns command history)
    LegacyDoorPanel legacyPanel;  // Adaptee (outlives the adapter)
    BuildingAccessService access; // owns the Adapter throug AccessController
    NotificationService notifier;
    LockdownFacade facade; // Facade over access + notifier + audit

    CampusWorld() : board(audit), security("Campus Security"), medical("Medical Response"), facilities("Facilities"), communications("Communications"), access(makeController(legacyPanel)), facade(access, notifier, audit)
    {
        // colleagues only know the mediator
        security.setMediator(&coordinator);
        medical.setMediator(&coordinator);
        facilities.setMediator(&coordinator);
        communications.setMediator(&coordinator);
        coordinator.setSecurity(&security);
        coordinator.setMedical(&medical);
        coordinator.setFacilities(&facilities);
        coordinator.setCommunications(&communications);

        access.addArea("ENG-ENTRANCE", "ENG", "Engineering Entrance", AREA_ENTRANCE);
        access.addArea("ENG-LAB", "ENG", "Engineering Laboratory", AREA_INTERIOR);
        access.addArea("ENG-OFFICE", "ENG", "Engineering Office", AREA_INTERIOR);
        access.addArea("LIB-ENTRANCE", "LIB", "Library Main Entrance", AREA_ENTRANCE);
        access.addArea("LIB-READING", "LIB", "Library Reading Room", AREA_INTERIOR);
    }

private:
    CampusWorld(const CampusWorld &);
    CampusWorld &operator=(const CampusWorld &);
};

// Drive State pattern
static void tryTransition(Incident &incident, const std::string &what)
{
    try
    {
        if (what == "dispatch")
            incident.dispatch();
        else if (what == "contain")
            incident.contain();
        else
            incident.resolve();
    }
    catch (const IncidentTransitionError &e)
    {
        std::cout << "[State] REJECTED:" << e.what() << "\n";
    }
}

static void printSummary(CampusWorld &w)
{
    step("SUMMARY");
    std::cout << "Commands held by invoker : " << w.console.commandCount() << "\n"
              << "Audit entries            : " << w.audit.entries().size() << "\n"
              << "Notifications sent       : " << w.notifier.sentCount() << "\n";
}
// Engineering lab fire. Command, Mediator, Adapter, Facade, State, Observer
static void scenarioFire(CampusWorld &w)
{
    banner("SCENARIO 1: LABORATORY FIRE -> ENGINEERING BUILDING");

    step("1. Incident reported (Registry + Observer attached)");
    Incident &fire = w.registry.registerIncident("Engineering Building", "Laboratory fire");
    fire.attach(&w.board);
    const int id = fire.getId();
    const std::string ref = "INC-" + std::to_string(id);

    step("2. Operator dispatches Security (Command -> Receiver -> Mediator)");
    w.console.execute(std::unique_ptr<Command>(new DispatchUnitCommand(w.registry, w.security, id)));

    step("3. Operator secures the lab (Command -> Facilities -> Mediator -> Security + Comms)");
    w.console.execute(std::unique_ptr<Command>(new SecureAreaCommand(w.registry, w.facilities, id, "Engineering Lab")));

    step("4. Operator issues evacuation alert (Command -> Comms -> Mediator -> all units)");
    w.console.execute(std::unique_ptr<Command>(new IssueAlertCommand(w.registry, w.communications, id, "Evacuate Engineering Building immediately.")));

    step("5. Facade: full building lockdown through the legacy panel Adapter");
    LockdownReport lock = w.facade.initiateLockdown("ENG", ref);
    std::cout << "Lockdown secured=" << lock.secured << " failed=" << lock.failed.size() << "\n";

    step("6. Operator cancels the last action (CancelActionCommand undoes the alert)");
    Command *last = w.console.lastCommand();
    if (last)
        w.console.execute(std::unique_ptr<Command>(new CancelActionCommand(*last)));

    step("7. Lifecycle progresses (State + Observer notify the board)");
    tryTransition(fire, "contain");
    tryTransition(fire, "resolve");
    std::cout << "Incident now: " << fire.describe() << "\n";

    step("8. Lift lockdown (Facade -> Adapter -> legacy panel)");
    LockdownReport lift = w.facade.liftLockdown("ENG", ref);
    std::cout << "Release secured=" << lift.secured << " failed=" << lift.failed.size() << "\n";

    step("9. Invalid operation: dispatch to a RESOLVED incident");
    w.console.execute(std::unique_ptr<Command>(new DispatchUnitCommand(w.registry, w.security, id)));

    printSummary(w);
}

// Library medical emergency withlegacy hardware fault Command, Mediator, Adapter,Facade, State,Observerr
static void scenarioMedical(CampusWorld &w)
{
    banner("SCENARIO 2: MEDICAL EMERGENCY -> LIBRARY (LEGACY PANEL FAULT)");

    step("1. Incident reported");
    Incident &med = w.registry.registerIncident("Library Reading Room", "Student collapsed, unresponsive");
    med.attach(&w.board);
    const int id = med.getId();
    const std::string ref = "INC-" + std::to_string(id);

    step("2. Operator dispatches Medical (Command -> Mediator -> Security + Facilities + Comms)");
    w.console.execute(std::unique_ptr<Command>(new DispatchUnitCommand(w.registry, w.medical, id)));

    step("3. Operator restricts the reading room (Command -> Facilities -> Mediator)");
    w.console.execute(std::unique_ptr<Command>(new SecureAreaCommand(w.registry, w.facilities, id, "Library Reading Room")));

    step("4. Hardware fault: reading-room legacy zone 202 goes OFFLINE");
    w.legacyPanel.setZoneOffline(202, true);

    step("5. Facade lockdown - Adapter translates the panels error code to a handled failure");
    LockdownReport lock = w.facade.initiateLockdown("LIB", ref);
    std::cout << "Lockdown secured=" << lock.secured << " failed=" << lock.failed.size();
    if (!lock.failed.empty())
        std::cout << " (manual check: " << lock.failed[0] << ")";
    std::cout << "\n";

    step("6. Panel repaired: operator retries via the Facade");
    w.legacyPanel.setZoneOffline(202, false);
    LockdownReport retry = w.facade.initiateLockdown("LIB", ref);
    std::cout << "Retry secured=" << retry.secured << " failed=" << retry.failed.size() << "\n";

    step("7. Invalid operations handled sensibly, not silently");
    std::cout << "(a) Dispatch to a non-existent incident:\n";
    w.console.execute(std::unique_ptr<Command>(new DispatchUnitCommand(w.registry, w.medical, 9999)));
    std::cout << "(b) Resolve while still ACTIVE (State rejects it):\n";
    tryTransition(med, "resolve");
    std::cout << "(c) Lockdown of an unknown building:\n";
    LockdownReport bad = w.facade.initiateLockdown("XYZ", ref);
    std::cout << "Unknown-building failures=" << bad.failed.size() << "\n";

    step("8. Patient stabilised; lifecycle completes (State + Observer)");
    tryTransition(med, "contain");
    tryTransition(med, "resolve");
    std::cout << "Incident now: " << med.describe() << "\n";

    step("9. Lift lockdown");
    LockdownReport lift = w.facade.liftLockdown("LIB", ref);
    std::cout << "Release secured=" << lift.secured << " failed=" << lift.failed.size() << "\n";

    printSummary(w);
}

static void printMenu()
{
    std::cout << "|CAMPUSGUARD  MAIN MENU                   |\n"
              << "|1  Run BOTH scenarios                    |\n"
              << "|2  Scenario 1: Engineering lab fire      |\n"
              << "|3  Scenario 2: Library medical emergency |\n"
              << "|0  Quit                                  |\n"
              << "Select an option: " << std::flush;
}

int main()
{
    std::cout << " CAMPUSGUARD: EMERGENCY RESPONSE PLATFORM\n"
              << "Patterns: Command, Mediator, Adapter, Facade, State, Observer\n";
    CampusWorld world;
    std::string line;
    bool everRan = false;
    while (true)
    {
        printMenu();
        if (!std::getline(std::cin, line))
        {
            std::cout << "\n[No interactive input detected]\n";
            if (!everRan)
            {
                scenarioFire(world);
                scenarioMedical(world);
            }
            break;
        }

        if (line == "1")
        {
            scenarioFire(world);
            scenarioMedical(world);
            everRan = true;
        }
        else if (line == "2")
        {
            scenarioFire(world);
            everRan = true;
        }
        else if (line == "3")
        {
            scenarioMedical(world);
            everRan = true;
        }
        else if (line == "0")
        {
            break;
        }
        else
        {
            std::cout << "Invalid option '" << line << "\n";
        }
    }

    std::cout << "\nCampusGuard down\n";
    return 0;
}