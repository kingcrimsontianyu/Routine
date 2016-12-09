#ifndef ROUTINE_SCORER_H
#define ROUTINE_SCORER_H 1

#include "G4PSEnergyDeposit.hh"

class RoutinePSEnergyDeposit : public G4PSEnergyDeposit
{
public:
    RoutinePSEnergyDeposit(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel);
    virtual ~RoutinePSEnergyDeposit();

protected:
    virtual G4int GetIndex(G4Step*);
    G4int fXNumVoxel;
    G4int fYNumVoxel;
    G4int fZNumVoxel;
};
#endif

