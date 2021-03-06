#include "RoutineSteppingAction.hh"
#include "RoutineEventAction.hh"
#include "RoutineDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "RoutineAnalysisManager.hh"

//------------------------------------------------------------
// stepping action class must be created with event action parameter
// because per-step energy is accumulated by event action function
// AddEdep()
//------------------------------------------------------------
RoutineSteppingAction::RoutineSteppingAction(RoutineEventAction* eventAction, RoutineUtility* rut_ext) :
G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(0), rut(rut_ext)
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

        rut->AccumulateEnergyByWorker(step);

        // // for primary particles
        // G4Track* aTrack = step->GetTrack();
        // if(aTrack->GetTrackID() == 1)
        // {
            // RoutineRun* run = static_cast<RoutineRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
            // auto process = step->GetPostStepPoint()->GetProcessDefinedStep();
            // if(process)
            // {
                // run->AddShortestStepProcess(process->GetProcessName());
            // }

            // // get polar angle distribution for scattering
            // if(process)
            // {
                // G4String procName = process->GetProcessName();
                // if((rut->GetParameter()->param->physics == "multiple-scattering-proton") ||
                   // (rut->GetParameter()->param->physics == "single-scattering-proton" &&
                   // procName == "CoulombScat"))
                // {
                    // G4StepPoint* prePoint  = step->GetPreStepPoint();
                    // G4StepPoint* postPoint = step->GetPostStepPoint();

                    // G4ThreeVector direction1 = prePoint->GetMomentumDirection();
                    // G4ThreeVector direction2 = postPoint->GetMomentumDirection();

                    // G4double polarAngleCosine = direction1.dot(direction2); // direction vector length == 1.0
                    // // handle numerical inaccuracy
                    // if(polarAngleCosine > 1.0)
                    // {
                        // polarAngleCosine = 1.0;
                    // }
                    // else if(polarAngleCosine < -1.0)
                    // {
                        // polarAngleCosine = -1.0;
                    // }

                    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                    // analysisManager->FillH1(0, polarAngleCosine);
                // }
            // }
        // }
    }
}


