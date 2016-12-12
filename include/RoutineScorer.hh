#ifndef ROUTINE_SCORER_H
#define ROUTINE_SCORER_H 1

#include "G4PSEnergyDeposit.hh"
#include "G4SystemOfUnits.hh"

//------------------------------------------------------------
//------------------------------------------------------------
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

//------------------------------------------------------------
// kerma = energy transfer / mass
// definition: kinetic energy of the secondary charged particles
// transferred from the primary uncharged particles
//
// note:
// --> for photons
//     --- photoelectric absorption: photoelectrons + Auger electrons (if any)
//     --- Compton scattering: photoelectrons + Auger electrons (if any)
//     --- pair production: electrons + positrons
//------------------------------------------------------------
class RoutinePSEnergyTransfer : public G4VPrimitiveScorer
{
public:
    RoutinePSEnergyTransfer(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel);
    virtual ~RoutinePSEnergyTransfer();
    virtual void Initialize(G4HCofThisEvent*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    virtual void clear();

protected:
    virtual G4int GetIndex(G4Step*);
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    G4int HCID;
    G4THitsMap<G4double>* EvtMap;
    G4int fXNumVoxel;
    G4int fYNumVoxel;
    G4int fZNumVoxel;
};
#endif

