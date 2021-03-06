#ifndef ROUTINE_SCORER_H
#define ROUTINE_SCORER_H 1

#include "G4PSEnergyDeposit.hh"
#include "G4SystemOfUnits.hh"
#include "G4PSTrackLength.hh"

//------------------------------------------------------------
//------------------------------------------------------------
class RoutinePSEnergyImparted : public G4PSEnergyDeposit
{
public:
    RoutinePSEnergyImparted(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel);
    virtual ~RoutinePSEnergyImparted();

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

//------------------------------------------------------------
// score track-length for primary particles
// The reason I did not just subclass G4PSTrackLength is
// that G4PSTrackLength has some private members that cannot
// be used, an annoying design limitation.
//------------------------------------------------------------
class RoutinePSPrimaryTrackLength : public G4VPrimitiveScorer
{
public:
    RoutinePSPrimaryTrackLength(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel);
    virtual ~RoutinePSPrimaryTrackLength();
    virtual void Initialize(G4HCofThisEvent*);
protected:
    virtual G4int GetIndex(G4Step*);
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

    G4int HCID;
    G4THitsMap<G4double>* EvtMap;
    G4int fXNumVoxel;
    G4int fYNumVoxel;
    G4int fZNumVoxel;
};

#endif

