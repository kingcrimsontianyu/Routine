#include "RoutinePrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePrimaryGeneratorAction::RoutinePrimaryGeneratorAction(RoutineParameterManager* rp)
: G4VUserPrimaryGeneratorAction(), rp(rp), fParticleGun(0)
{
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle;
	
	G4cout << ">>> " << rp->param->usePDGEncoding << G4endl;
	G4cout << ">>> " << rp->param->PDGEncoding << G4endl;
	G4cout << ">>> " << rp->param->sourceType << G4endl;
    if(rp->param->usePDGEncoding)
    {
        particle = particleTable->FindParticle(rp->param->PDGEncoding);
    }
    else
    {
        particle = particleTable->FindParticle(rp->param->sourceType);
    }
    fParticleGun->SetParticleDefinition(particle);

    G4ThreeVector dir = {rp->param->sourceDirection.x, rp->param->sourceDirection.y, rp->param->sourceDirection.z};
    fParticleGun->SetParticleMomentumDirection(dir);

    fParticleGun->SetParticleEnergy(rp->param->sourceEnergy);
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutinePrimaryGeneratorAction::~RoutinePrimaryGeneratorAction()
{
    delete fParticleGun;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutinePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // square pencil beam
    G4double dx = (G4UniformRand() - 0.5) * rp->param->squareBeamWidth;
    G4double dy = (G4UniformRand() - 0.5) * rp->param->squareBeamWidth;
    G4ThreeVector position = {dx, dy, rp->param->sourcePosition.z};
    fParticleGun->SetParticlePosition(position);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}



