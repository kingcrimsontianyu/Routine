#ifndef ROUTINE_UTILITY_H
#define ROUTINE_UTILITY_H

#include "globals.hh"
#include "g4csv.hh"
#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"
#include "G4UserSteppingAction.hh"
#include "G4UserTrackingAction.hh"
#include "G4UserStackingAction.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "RoutineParameter.hh"
#ifdef G4MULTITHREADED
    #include "G4MTRunManager.hh"
#else
    #include "G4RunManager.hh"
#endif
#include "RoutineDetectorConstruction.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VMultipleScattering.hh"
#include "G4HadronicProcess.hh"
#include "G4AutoLock.hh"

class G4HistManager;
class RoutineRun;

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineUtility
{
public:
    //------------------------------------------------------------
    // member functions for master thread only
    //------------------------------------------------------------
    RoutineUtility(RoutineParameterManager* rp);
    RoutineUtility();
    ~RoutineUtility();

    // This function shall be used to determine whether particle info is to be printed at runtime.
    void SetPrintParticleInfoByMaster(G4bool choose);

    // This function shall be called in user's G4UserRunAction::BeginOfRunAction(const G4Run*) method
    // to print all particles and their associated processes for a given physics list.
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Some ions such as O16 are assembled at run-time and are not included in the particle table before
    // the run
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void PrintPhysicsInfoByMaster();

    void PrintVisualInfoByMaster();

    void SaveCustomScoreToFileByMaster();

    G4VUserPhysicsList* ChoosePhysicsByMaster();





    //------------------------------------------------------------
    // member functions for worker threads
    //------------------------------------------------------------
    // This function shall be called in user's G4UserStackingAction::ClassifyNewTrack(const G4Track* track) method
    // to print particle information when the particle is about to be simulated. By default this function is not
    // effective (bPrintParticleInfo == false). Use SetPrintParticleInfo(true) to activate this function.
    void PrintParticleInfoByWorker(const G4Track* track);

    // called by G4UserStackingAction::ClassifyNewTrack()
    void AccumulateCountByWorker(const G4Track* track);

    // called by G4UserSteppingAction::UserSteppingAction()
    void AccumulateEnergyByWorker(const G4Step* step);


protected:
    G4bool bPrintParticleInfo;
    std::vector<G4String> supportedParticleList;
    std::vector<G4String> activeParticleList;
    RoutineParameterManager* rp;
    G4Mutex mut;
};

//------------------------------------------------------------
//------------------------------------------------------------
struct ProcessBlob
{
    ProcessBlob();
    G4String processName;
    std::vector<G4String> modelName;
    G4bool hasOccurred;
};

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineCustomScore
{
public:
    RoutineCustomScore(G4double count, G4double energy);
    RoutineCustomScore();
    ~RoutineCustomScore();
    G4double energy;
    G4double count;
    std::vector<ProcessBlob> processList;
    std::vector<ProcessBlob> processForEnergyList;

    RoutineCustomScore& operator += (const RoutineCustomScore& rhs);
    RoutineCustomScore& operator = (const RoutineCustomScore& rhs);
    void AccumulateEnergy(const G4double energy);
    void AccumulateCount(const G4double count);
};

#endif

