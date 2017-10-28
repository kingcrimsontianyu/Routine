#include "RoutineTrackingAction.hh"
#include "G4Track.hh"
#include "RoutineRun.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineTrackingAction::RoutineTrackingAction(RoutinePrimaryGeneratorAction* primaryGen) :
G4UserTrackingAction(),
fPrimaryGen(primaryGen)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineTrackingAction::~RoutineTrackingAction()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineTrackingAction::PreUserTrackingAction(const G4Track*)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    // for primary particles
    if (aTrack->GetTrackID() == 1)
    {
        G4double trackLength = aTrack->GetTrackLength();
        RoutineRun* run = static_cast<RoutineRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
        G4ThreeVector vertex = fPrimaryGen->GetParticleGun()->GetParticlePosition();
        // particle is shooting toward negative y axis
        G4ThreeVector displace = vertex - aTrack->GetPosition();


        run->AddTrueRange(trackLength);
        run->AddProjectedRange(displace.y());
    }
}

