#include "Routine.hh"
#include "RoutineDetectorConstruction.hh"
#include "RoutineActionInitialization.hh"
#include "RoutineParameter.hh"

#ifdef G4MULTITHREADED
    #include "G4MTRunManager.hh"
#else
    #include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "RoutinePhysics.hh"
#include "G4Timer.hh"

// #define USE_GUI

//------------------------------------------------------------
//------------------------------------------------------------
int main(int argc,char** argv)
{
    G4Timer timer;
    timer.Start();

    RoutineParameterManager* rp = new RoutineParameterManager(argc, argv);

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
    runManager->SetNumberOfThreads(rp->param->numThread);
    #else
    G4RunManager* runManager = new G4RunManager;
    #endif

    RoutineUtility* rut = new RoutineUtility(rp);
    // rut->SetPrintParticleInfo(true);

    runManager->SetUserInitialization(new RoutineDetectorConstruction(rp));

    G4VUserPhysicsList* physicsList = new QBBC;
    // G4VUserPhysicsList* physicsList = new RoutineQBBC;
    // G4VUserPhysicsList* physicsList = new RoutineMinimalist;
    // G4VUserPhysicsList* physicsList = new RoutineTopas;
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new RoutineActionInitialization(rp, rut));

    runManager->Initialize();

    // must be called after G4RunManager->Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if(rp->param->isIonSource)
    {
        UImanager->ApplyCommand("/gun/particle ion");
        UImanager->ApplyCommand("/gun/ion " + std::to_string(rp->param->sourceIonZ) + " " + std::to_string(rp->param->sourceIonA));

        // UImanager->ApplyCommand("/control/execute " + rp->param->macroFilePath);
    }
    // UImanager->ApplyCommand("/process/setVerbose 2");
    // UImanager->ApplyCommand("/control/verbose 2");
    // UImanager->ApplyCommand("/run/particle/verbose 3");
    // UImanager->ApplyCommand("/event/verbose 2");
    // UImanager->ApplyCommand("/tracking/verbose 2");
    // UImanager->ApplyCommand("/hits/verbose 2");
    // UImanager->ApplyCommand("/particle/verbose 2");


    runManager->BeamOn(rp->param->numHistory);

    #if defined USE_GUI
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    rut->PrintVisualInfo();
    #endif

    #if defined USE_GUI
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif

    if(rp->param->storePhysicsTable)
    {
        UImanager->ApplyCommand("/run/particle/storePhysicsTable " + rp->param->physicsTableDir);
    }

    #if defined USE_GUI
    ui->SessionStart();
    delete ui;
    delete visManager;
    #endif

    delete rut;
    delete runManager;

    delete rp;

    timer.Stop();
    G4cout << "--> total real elapsed time is: "<< timer.GetRealElapsed() << G4endl;
    G4cout << "    total system elapsed time: " << timer.GetSystemElapsed() << G4endl;
    G4cout << "    total user elapsed time: " << timer.GetUserElapsed() << G4endl;

    return 0;
}




