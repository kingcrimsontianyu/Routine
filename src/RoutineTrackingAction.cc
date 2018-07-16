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
    // // for primary particles
    // if (aTrack->GetTrackID() == 1)
    // {
        // G4double trackLength = aTrack->GetTrackLength();
        // RoutineRun* run = static_cast<RoutineRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
        // G4ThreeVector initialPosition = fPrimaryGen->GetParticleGun()->GetParticlePosition();
        // G4ThreeVector finalPosition = aTrack->GetPosition();
        // // particle is shooting toward negative y axis
        // G4ThreeVector displace = initialPosition - finalPosition;
        // run->AddTrueRange(trackLength);
        // run->AddProjectedRange(displace.y());

        // G4double lateral = finalPosition.x() * finalPosition.x() + finalPosition.z() * finalPosition.z();
        // lateral = std::sqrt(lateral);
        // G4double polarAngleTan = lateral / std::fabs(displace.y());
        // run->AddPolarAngle(std::atan(polarAngleTan));
    // }
}

