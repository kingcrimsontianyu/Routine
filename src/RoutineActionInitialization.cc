#include "RoutineActionInitialization.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::RoutineActionInitialization(RoutineParameterManager* rp_ext, RoutineUtility* rut_ext) :
G4VUserActionInitialization(), rp(rp_ext), rut(rut_ext)
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::~RoutineActionInitialization()
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineActionInitialization::BuildForMaster() const
{
    SetUserAction(new RoutineRunAction(rp, rut));
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineActionInitialization::Build() const
{
    // primary generation class
    RoutinePrimaryGeneratorAction* primaryGen = new RoutinePrimaryGeneratorAction(rp);
    SetUserAction(primaryGen);

    // user run action class
    RoutineRunAction* runAction = new RoutineRunAction(rp, rut);
    SetUserAction(runAction);

    // user event action class
    RoutineEventAction* eventAction = new RoutineEventAction(runAction);
    SetUserAction(eventAction);

    // user stepping action class (delta track)
    RoutineSteppingAction* steppingAction = new RoutineSteppingAction(eventAction, rut);
    SetUserAction(steppingAction);

    // user stacking action class
    RoutineStackingAction* stackingAction = new RoutineStackingAction(rut);
    SetUserAction(stackingAction);

    // user tracking action class
    RoutineTrackingAction* trackingAction = new RoutineTrackingAction(primaryGen);
    SetUserAction(trackingAction);
}

