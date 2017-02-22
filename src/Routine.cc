#include "Routine.hh"
// #define USE_GUI

//------------------------------------------------------------
//------------------------------------------------------------
int main(int argc,char** argv)
{
    #if defined USE_GUI
    G4UIExecutive* ui = 0;
    if ( argc == 1 )
    {
        ui = new G4UIExecutive(argc, argv);
    }
    #endif

    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    #if defined G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(28);
    #else
    G4RunManager* runManager = new G4RunManager;
    #endif

    RoutineUtility* rut = new RoutineUtility();
    // rut->SetPrintParticleInfo(true);

    runManager->SetUserInitialization(new RoutineDetectorConstruction());

    // G4VUserPhysicsList* physicsList = new QBBC;
    // G4VUserPhysicsList* physicsList = new RoutineQBBC;
    // G4VUserPhysicsList* physicsList = new RoutineMinimalist;
    G4VUserPhysicsList* physicsList = new RoutineTopas;
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new RoutineActionInitialization(rut));

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/hits/verbose 0");

    runManager->Initialize();
    int numberOfEvent = static_cast<int>(1e3);
    runManager->BeamOn(numberOfEvent);

    #if defined USE_GUI
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    rut->PrintVisualInfo();
    #endif

    #if defined USE_GUI
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif

    // UImanager->ApplyCommand("/run/particle/storePhysicsTable xs_data");

    #if defined USE_GUI
    ui->SessionStart();
    delete ui;
    delete visManager;
    #endif

    delete rut;
    delete runManager;
}




