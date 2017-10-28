#ifndef ROUTINE_ACTIONINITIALIZATION_H
#define ROUTINE_ACTIONINITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"
#include "RoutinePrimaryGeneratorAction.hh"
#include "RoutineRunAction.hh"
#include "RoutineEventAction.hh"
#include "RoutineSteppingAction.hh"
#include "RoutineStackingAction.hh"
#include "RoutineTrackingAction.hh"
#include "RoutineRun.hh"
#include "RoutineParameter.hh"

class RoutineActionInitialization : public G4VUserActionInitialization
{
public:
    RoutineActionInitialization(RoutineParameterManager* rp, RoutineUtility* rut);
    virtual ~RoutineActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
protected:
    RoutineParameterManager* rp;
    RoutineUtility* rut;
};

#endif


