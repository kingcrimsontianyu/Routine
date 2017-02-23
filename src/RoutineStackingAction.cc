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
    #if defined G4MULTITHREADED
    G4RunManager* rm = G4RunManager::GetRunManager();
    #else
    G4MTRunManager* rm = G4MTRunManager::GetMasterRunManager();
    #endif

    G4Run* run = rm->GetNonConstCurrentRun();
    RoutineRun* localRun = static_cast<RoutineRun*>(run);
    rut->AccumulateCount(localRun, track);

    G4ClassificationOfNewTrack classification = fUrgent;
    if(track->GetTrackStatus() == fPostponeToNextEvent)
    {
        classification = fPostpone;
    }
    return classification;
}

