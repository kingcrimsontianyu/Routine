#include "Routine.hh"
// #define USE_GUI

//------------------------------------------------------------
//------------------------------------------------------------
int main(int argc,char** argv)
{
    G4Timer timer;
    timer.Start();

    // very verbose in order to glean useful cross-section info
    // G4EmParameters* empar = G4EmParameters::Instance();
    // empar->SetVerbose(3);

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

    G4VUserPhysicsList* physicsList = new QBBC;
    // G4VUserPhysicsList* physicsList = new RoutineQBBC;
    // G4VUserPhysicsList* physicsList = new RoutineMinimalist;
    // G4VUserPhysicsList* physicsList = new RoutineTopas;
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new RoutineActionInitialization(rut));

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    // UImanager->ApplyCommand("/process/setVerbose 2");
    // UImanager->ApplyCommand("/control/verbose 2");
    // UImanager->ApplyCommand("/run/particle/verbose 3");
    // UImanager->ApplyCommand("/event/verbose 2");
    // UImanager->ApplyCommand("/tracking/verbose 2");
    // UImanager->ApplyCommand("/hits/verbose 2");
    // UImanager->ApplyCommand("/particle/verbose 2");

    runManager->Initialize();
    int numberOfEvent = static_cast<int>(1e6);
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

    timer.Stop();
    G4cout << "--> total real elapsed time is: "<< timer.GetRealElapsed() << G4endl;
    G4cout << "    total system elapsed time: " << timer.GetSystemElapsed() << G4endl;
    G4cout << "    total user elapsed time: " << timer.GetUserElapsed() << G4endl;
}




