#include "RoutineActionInitialization.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::RoutineActionInitialization() : G4VUserActionInitialization()
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineActionInitialization::~RoutineActionInitialization()
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineActionInitialization::BuildForMaster() const
{
    SetUserAction(new RoutineRunAction);
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineActionInitialization::Build() const
{
    // primary generation class
    SetUserAction(new RoutinePrimaryGeneratorAction);

    // user run action class
    RoutineRunAction* runAction = new RoutineRunAction;
    SetUserAction(runAction);

    // user event action class
    RoutineEventAction* eventAction = new RoutineEventAction(runAction);
    SetUserAction(eventAction);

    // user stepping action class (delta track)
    SetUserAction(new RoutineSteppingAction(eventAction));
}

