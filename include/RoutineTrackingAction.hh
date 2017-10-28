#ifndef ROUTINE_TRACKING_ACTION_H
#define ROUTINE_TRACKING_ACTION_H

#include "G4UserTrackingAction.hh"
#include "RoutinePrimaryGeneratorAction.hh"

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineTrackingAction : public G4UserTrackingAction
{
public:
    RoutineTrackingAction(RoutinePrimaryGeneratorAction* primaryGen);
    virtual ~RoutineTrackingAction();
    virtual void PreUserTrackingAction(const G4Track* aTrack) override;
    virtual void PostUserTrackingAction(const G4Track* aTrack) override;
protected:
    RoutinePrimaryGeneratorAction* fPrimaryGen;
};

#endif

