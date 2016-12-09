#ifndef ROUTINE_STEPPINGACTION_H
#define ROUTINE_STEPPINGACTION_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class RoutineEventAction;

class G4LogicalVolume;

class RoutineSteppingAction : public G4UserSteppingAction
{
public:
    RoutineSteppingAction(RoutineEventAction* eventAction);
    virtual ~RoutineSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

private:
    RoutineEventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
};

#endif
