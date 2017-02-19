#include "RoutineScorer.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePSEnergyImparted::RoutinePSEnergyImparted(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel)
:G4PSEnergyDeposit(name),
fXNumVoxel(xNumVoxel),
fYNumVoxel(yNumVoxel),
fZNumVoxel(zNumVoxel)
{}

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePSEnergyImparted::~RoutinePSEnergyImparted()
{}

//------------------------------------------------------------
//------------------------------------------------------------
G4int RoutinePSEnergyImparted::GetIndex(G4Step* aStep)
{
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int xIdx = touchable->GetReplicaNumber(2);
    G4int yIdx = touchable->GetReplicaNumber(1);
    G4int zIdx = touchable->GetReplicaNumber(0);

    // x idx changes fastest, z slowest
    G4int globalIdx = fXNumVoxel * fYNumVoxel * zIdx + fXNumVoxel * yIdx + xIdx;
    return globalIdx;
}


//------------------------------------------------------------
//------------------------------------------------------------
RoutinePSEnergyTransfer::RoutinePSEnergyTransfer(G4String name, G4int xNumVoxel, G4int yNumVoxel, G4int zNumVoxel)
: G4VPrimitiveScorer(name),
HCID(-1), EvtMap(0), fXNumVoxel(xNumVoxel), fYNumVoxel(yNumVoxel), fZNumVoxel(zNumVoxel)
{
    CheckAndSetUnit("MeV", "Energy");
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePSEnergyTransfer::~RoutinePSEnergyTransfer()
{}

//------------------------------------------------------------
//------------------------------------------------------------
G4int RoutinePSEnergyTransfer::GetIndex(G4Step* aStep)
{
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int xIdx = touchable->GetReplicaNumber(2);
    G4int yIdx = touchable->GetReplicaNumber(1);
    G4int zIdx = touchable->GetReplicaNumber(0);

    // x idx changes fastest, z slowest
    G4int globalIdx = fXNumVoxel * fYNumVoxel * zIdx + fXNumVoxel * yIdx + xIdx;
    return globalIdx;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4bool RoutinePSEnergyTransfer::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // // manual verbose check
    // // for(auto it = secondaryList->begin(); it != secondaryList->end(); ++it)
    // // {
        // // G4cout << "--> secondary particle in current step = " << (*it)->GetDefinition()->GetParticleName() << G4endl;
    // // }

    G4double energyTransfer = 0.0;
    G4Track* track = aStep->GetTrack();
    G4int id = track->GetTrackID();
    G4double charge = track->GetParticleDefinition()->GetPDGCharge();

    // score if
    // --- the particle is primary particle
    // --- the particle is uncharged particle
    if(id == 1 && charge == 0.0)
    {
        const std::vector<const G4Track*>* secondaryList = aStep->GetSecondaryInCurrentStep();
        for(auto it = secondaryList->begin(); it != secondaryList->end(); ++it)
        {
            G4double secondaryCharge = (*it)->GetParticleDefinition()->GetPDGCharge();
            // score if the secondary particle is charged particle
            if(secondaryCharge != 0.0)
            {
                G4double K = (*it)->GetKineticEnergy();
                energyTransfer += K;
            }
        }
    }

    if(energyTransfer == 0.0 )
    {
        return FALSE;
    }
    else
    {
        energyTransfer *= aStep->GetPreStepPoint()->GetWeight();
        G4int index = GetIndex(aStep);
        EvtMap->add(index, energyTransfer);
        return TRUE;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutinePSEnergyTransfer::Initialize(G4HCofThisEvent* HCE)
{
    EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
    if(HCID < 0)
    {
        HCID = GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutinePSEnergyTransfer::EndOfEvent(G4HCofThisEvent*)
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutinePSEnergyTransfer::clear()
{
    EvtMap->clear();
}







