#ifndef ROUTINE_UTILITY_H
#define ROUTINE_UTILITY_H

#include "globals.hh"
#include "g4root.hh"
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
#ifdef G4MULTITHREADED
    #include "G4MTRunManager.hh"
#else
    #include "G4RunManager.hh"
#endif

class G4HistManager;

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineUtility
{
public:
    RoutineUtility();
    ~RoutineUtility();

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // runtime print
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // This function shall be called in user's G4UserStackingAction::ClassifyNewTrack(const G4Track* track) method
    // to print particle information when the particle is about to be simulated.
    void PrintParticleInfo(const G4Track* track);

    // This function shall be used to determine whether particle info is to be printed at runtime.
    void SetPrintParticleInfo(G4bool choose);

    // This function shall be called in user's G4UserRunAction::BeginOfRunAction(const G4Run*) method
    // to print all particles and their associated processes for a given physics list.
    void PrintPhysicsInfo();

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // runtime tally
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    G4HistManager* CreateHistManager();
    void DeleteHistManager();
    G4HistManager* GetHistManager();
    G4AnalysisManager* GetAnalysisManager();
    void AccumulateCounter(const G4Track* track);
    void PrintVisualInfo();
    void SaveHistToFile();
protected:
    G4HistManager* fHistManager;
    G4bool bPrintParticleInfo;
};

//------------------------------------------------------------
//------------------------------------------------------------
class G4HistManager
{
public:
    G4HistManager();
    ~G4HistManager();
    void SetUpHist();
private:
    G4String fFileName;
};

#endif

