#include "RoutineEventAction.hh"
#include "RoutineRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineEventAction::RoutineEventAction(RoutineRunAction* runAction) : G4UserEventAction(),
                                                                   fRunAction(runAction),
                                                                   fEdep(0.0)
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineEventAction::~RoutineEventAction()
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.0;
}

//------------------------------------------------------------
// after each single particle has been simulated
//------------------------------------------------------------
void RoutineEventAction::EndOfEventAction(const G4Event*)
{
    // accumulate statistics in run action
    fRunAction->AddEdep(fEdep);
}

//------------------------------------------------------------
// during simulation of a particle, accumulate scoring
// this function is called by UserSteppingAction()
//------------------------------------------------------------
void RoutineEventAction::AddEdep(G4double edep)
{
    fEdep += edep;
}


