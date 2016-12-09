#include "RoutineSteppingAction.hh"
#include "RoutineEventAction.hh"
#include "RoutineDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//------------------------------------------------------------
// stepping action class must be created with event action parameter
// because per-step energy is accumulated by event action function
// AddEdep()
//------------------------------------------------------------
RoutineSteppingAction::RoutineSteppingAction(RoutineEventAction* eventAction) : G4UserSteppingAction(),
                                                                             fEventAction(eventAction),
                                                                             fScoringVolume(0)
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineSteppingAction::~RoutineSteppingAction()
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineSteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fScoringVolume)
    {
        auto detectorConstruction = static_cast<const RoutineDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        // if the particle is in the phantom, it is considered to be inside the logical volume with highest depth value 2
        fScoringVolume = detectorConstruction->GetLogicVolumeHighestDepthInPhantom();
    }

    // get volume of the current step
    G4TouchableHandle touch = step->GetPreStepPoint()->GetTouchableHandle();
    G4LogicalVolume* volume = touch->GetVolume()->GetLogicalVolume();

    // check if we are in scoring volume
    if (volume == fScoringVolume)
    {
        // collect energy deposited in this step
        G4double edepStep = step->GetTotalEnergyDeposit();
        fEventAction->AddEdep(edepStep);
    }
}


