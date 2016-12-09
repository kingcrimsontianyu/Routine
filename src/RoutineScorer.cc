#include "RoutineScorer.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePSEnergyDeposit::RoutinePSEnergyDeposit(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel)
:G4PSEnergyDeposit(name),
fXNumVoxel(xNumVoxel),
fYNumVoxel(yNumVoxel),
fZNumVoxel(zNumVoxel)
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePSEnergyDeposit::~RoutinePSEnergyDeposit()
{}

//------------------------------------------------------------
//------------------------------------------------------------
G4int RoutinePSEnergyDeposit::GetIndex(G4Step* aStep)
{
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int xIdx = touchable->GetReplicaNumber(2);
    G4int yIdx = touchable->GetReplicaNumber(1);
    G4int zIdx = touchable->GetReplicaNumber(0);

    // z idx changes fastest, x slowest
    G4int globalIdx = fZNumVoxel * fYNumVoxel * xIdx + fZNumVoxel * yIdx + zIdx;
    return globalIdx;
}
