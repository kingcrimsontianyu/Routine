#ifndef ROUTINE_STACKING_ACTION_H
#define ROUTINE_STACKING_ACTION_H

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "RoutineUtility.hh"
#include "RoutineRun.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

class G4Track;

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineStackingAction : public G4UserStackingAction
{
public:
    RoutineStackingAction(RoutineUtility* rut);
    virtual ~RoutineStackingAction();
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

protected:
    RoutineUtility* rut;
};

#endif

