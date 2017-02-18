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
    G4MTRunManager* runManager = new G4MTRunManager;

    RoutineUtility* rut = new RoutineUtility();
    rut->SetPrintParticleInfo(true);

    runManager->SetUserInitialization(new RoutineDetectorConstruction());
    runManager->SetUserInitialization(new RoutineStandard);
    // runManager->SetUserInitialization(new RoutineMinimalist);
    runManager->SetUserInitialization(new RoutineActionInitialization(rut));
    runManager->SetNumberOfThreads(1);

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/hits/verbose 0");

    runManager->Initialize();
    int numberOfEvent = static_cast<int>(1e2);
    runManager->BeamOn(numberOfEvent);

    #if defined USE_GUI
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    GetVisInfo(visManager);
    #endif

    #if defined USE_GUI
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif

    #if defined USE_GUI
    ui->SessionStart();
    delete ui;
    delete visManager;
    #endif

    delete rut;
    delete runManager;
}

//------------------------------------------------------------
//------------------------------------------------------------
void GetVisInfo(G4VisManager* visManager)
{
    // G4cout << visManager->GetCurrentGraphicsSystem()->GetName() << G4endl;

    auto gph = visManager->GetAvailableGraphicsSystems();

    for(size_t i = 0; i < gph.size(); ++i)
    {
        G4cout << "--> simple: " << gph[i]->GetName() << G4endl;
    }

    if(visManager->GetCurrentGraphicsSystem() == nullptr)
    {
        G4cout << "--> simple: current graphics system is nullptr XS." << G4endl;
    }

}














