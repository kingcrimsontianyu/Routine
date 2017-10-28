#ifndef ROUTINE_PRIMARYGENERATORACTION_H
#define ROUTINE_PRIMARYGENERATORACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "RoutineParameter.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class RoutinePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    RoutinePrimaryGeneratorAction(RoutineParameterManager* rp);
    virtual ~RoutinePrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    G4ParticleGun* GetParticleGun();

private:
    RoutineParameterManager* rp;
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
};

#endif
