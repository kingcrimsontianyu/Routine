#ifndef ROUTINE_EVENTACTION_H
#define ROUTINE_EVENTACTION_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RoutineRunAction;

class RoutineEventAction : public G4UserEventAction
{
public:
    RoutineEventAction(RoutineRunAction* runAction);
    virtual ~RoutineEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    void AddEdep(G4double edep);

private:
    RoutineRunAction* fRunAction;
    G4double         fEdep;
};

#endif


