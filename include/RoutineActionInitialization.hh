#ifndef ROUTINE_ACTIONINITIALIZATION_H
#define ROUTINE_ACTIONINITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"
#include "RoutinePrimaryGeneratorAction.hh"
#include "RoutineRunAction.hh"
#include "RoutineEventAction.hh"
#include "RoutineSteppingAction.hh"
#include "RoutineStackingAction.hh"
#include "RoutineRun.hh"

class RoutineActionInitialization : public G4VUserActionInitialization
{
public:
    RoutineActionInitialization(RoutineUtility* rut);
    virtual ~RoutineActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
protected:
    RoutineUtility* rut;
};

#endif


