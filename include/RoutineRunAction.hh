#ifndef ROUTINE_RUNACTION_H
#define ROUTINE_RUNACTION_H 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "RoutinePrimaryGeneratorAction.hh"
#include "RoutineDetectorConstruction.hh"
#include "RoutineRun.hh"
#include "RoutinePhysics.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "RoutineUtility.hh"
#include "RoutineParameter.hh"

class G4Run;
class RoutineAnalysisManager;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RoutineRunAction : public G4UserRunAction
{
public:
    RoutineRunAction(RoutineParameterManager* rp, RoutineUtility* rut);
    virtual ~RoutineRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    void OutputVoxelTally(const G4String& prefix,
                          const G4int numHistory,
                          G4THitsMap<G4double>* hitsMap,
                          G4THitsMap<G4double>* hitsMapSquared,
                          G4bool isDividedByMass = true);
    void CalculateMeanAndRsd(G4double& mean,
                             G4double& rsd,
                             const G4int numHistory,
                             const G4double x,
                             const G4double x2);
    void AddEdep (G4double edep);
private:
    G4Accumulable<G4double> fEdep;
    G4Accumulable<G4double> fEdep2;
    RoutineParameterManager* rp;
    RoutineUtility* rut;
    RoutineAnalysisManager* fRoutineAnalysisManager;
};

#endif

