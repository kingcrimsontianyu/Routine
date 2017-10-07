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
#include "RoutineImportPhantom.hh"

//------------------------------------------------------------
//------------------------------------------------------------
int main(int argc, char** argv)
{
    Impl(argc, argv);
    return 0;
}

//------------------------------------------------------------
//------------------------------------------------------------
void Impl(int argc,char** argv)
{
    G4Timer timer;
    timer.Start();

    RoutineParameterManager* rp = new RoutineParameterManager(argc, argv);
    RoutineUtility* rut = new RoutineUtility(rp);
    G4UIExecutive* ui = nullptr;
    G4VisManager* visManager = nullptr;
    G4UImanager* UImanager = nullptr;

    // very verbose in order to glean useful cross-section info
    G4EmParameters* empar = G4EmParameters::Instance();
    empar->SetVerbose(3);

    if(rp->param->useUI)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    #if defined G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(rp->param->numThread);
    #else
    G4RunManager* runManager = new G4RunManager;
    #endif

    // rut->SetPrintParticleInfo(true);

    runManager->SetUserInitialization(new RoutineDetectorConstruction(rp));

    G4VUserPhysicsList* physicsList = rut->ChoosePhysicsByMaster();
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new RoutineActionInitialization(rp, rut));

    runManager->Initialize();

    // must be called after G4RunManager->Initialize();
    UImanager = G4UImanager::GetUIpointer();
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

    if(rp->param->useUI)
    {
        visManager = new G4VisExecutive;
        visManager->Initialize();
        rut->PrintVisualInfoByMaster();
        UImanager->ApplyCommand("/control/execute vis.mac");
    }

    if(rp->param->storePhysicsTable)
    {
        UImanager->ApplyCommand("/run/particle/storePhysicsTable " + rp->param->physicsTableDir);
    }

    if(rp->param->useUI)
    {
        ui->SessionStart();
        delete ui;
        delete visManager;
    }

    delete rut;
    delete runManager;

    delete rp;

    timer.Stop();
    G4cout << "--> Total real elapsed time = "<< timer.GetRealElapsed() << " [sec]" << G4endl;
}

