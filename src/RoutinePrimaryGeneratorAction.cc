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
RoutinePrimaryGeneratorAction::RoutinePrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(),
                                                               fParticleGun(0)
{
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    // G4ParticleDefinition* particle = particleTable->FindParticle("proton");
    G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0));
    // fParticleGun->SetParticleEnergy(200.0 * MeV);
    fParticleGun->SetParticleEnergy(6.0 * MeV);
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
    // pencil beam: 1 x 1 cm2 square
    G4double sigmaPosition = 0.1 * cm;
    G4double dx = (G4UniformRand() - 0.5) * sigmaPosition;
    G4double dy = (G4UniformRand() - 0.5) * sigmaPosition;
    G4ThreeVector position = {dx, dy, -30.0 * cm};
    fParticleGun->SetParticlePosition(position);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}



