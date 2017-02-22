#include "RoutineActionInitialization.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::RoutineActionInitialization(RoutineUtility* rut) :
G4VUserActionInitialization(), rut(rut)
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::~RoutineActionInitialization()
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineActionInitialization::BuildForMaster() const
{
    SetUserAction(new RoutineRunAction(rut));
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineActionInitialization::Build() const
{
    // primary generation class
    SetUserAction(new RoutinePrimaryGeneratorAction);

    // user run action class
    RoutineRunAction* runAction = new RoutineRunAction(rut);
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

