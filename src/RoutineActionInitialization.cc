#include "RoutineActionInitialization.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::RoutineActionInitialization(RoutineParameterManager* rp, RoutineUtility* rut) :
G4VUserActionInitialization(), rp(rp), rut(rut)
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
    SetUserAction(new RoutinePrimaryGeneratorAction(rp));

    // user run action class
    RoutineRunAction* runAction = new RoutineRunAction(rp, rut);
    SetUserAction(runAction);

    // user event action class
    RoutineEventAction* eventAction = new RoutineEventAction(runAction);
    SetUserAction(eventAction);

    // user stepping action class (delta track)
    SetUserAction(new RoutineSteppingAction(eventAction, rut));

    // user stacking action class
    RoutineStackingAction* stackingAction = new RoutineStackingAction(rut);
    SetUserAction(stackingAction);
}

