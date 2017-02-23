#include "RoutineStackingAction.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineStackingAction::RoutineStackingAction(RoutineUtility* rut) : G4UserStackingAction(), rut(rut)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineStackingAction::~RoutineStackingAction()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
G4ClassificationOfNewTrack RoutineStackingAction::ClassifyNewTrack(const G4Track* track)
{
    rut->AccumulateCount(track);

    G4ClassificationOfNewTrack classification = fUrgent;
    if(track->GetTrackStatus() == fPostponeToNextEvent)
    {
        classification = fPostpone;
    }
    return classification;
}

