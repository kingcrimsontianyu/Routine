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
#include "RoutineDetectorConstruction.hh"

class G4HistManager;
class RoutineRun;

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

    void PrintVisualInfo();

    void AccumulateCount(RoutineRun* localRun, const G4Track* track);
    void AccumulateEnergy(RoutineRun* localRun, const G4Step* step);
    void SaveCustomScoreToFile();
protected:
    G4bool bPrintParticleInfo;
    std::vector<G4String> supportedParticleList;
    std::vector<G4String> activeParticleList;
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

    RoutineCustomScore& operator += (const RoutineCustomScore& rhs);
    RoutineCustomScore& operator = (const RoutineCustomScore& rhs);
    void AccumulateEnergy(const G4double energy);
    void AccumulateCount(const G4double count);
};

#endif

