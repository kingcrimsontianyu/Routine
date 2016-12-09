#ifndef ROUTINE_PHYSICSLIST_H
#define ROUTINE_PHYSICSLIST_H

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

#include "G4ios.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4DataQuestionaire.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4hIonisation.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"

#include "QBBC.hh"

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineModularPhysics : public G4VModularPhysicsList
{
public:
    G4String name;
    RoutineModularPhysics() : G4VModularPhysicsList() {};
    ~RoutineModularPhysics() {};
};

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineUserPhysics : public G4VUserPhysicsList
{
public:
    G4String name;
    RoutineUserPhysics() : G4VUserPhysicsList() {};
    ~RoutineUserPhysics() {};
};

//******************************
// standard
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineStandard: public RoutineModularPhysics
{
public:
    RoutineStandard(G4int ver = 0);
    virtual ~RoutineStandard();
    virtual void SetCuts();
};

//******************************
// minimalist
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineMinimalist: public RoutineUserPhysics
{
public:
    RoutineMinimalist(G4int ver = 0);
    virtual ~RoutineMinimalist();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    virtual void SetCuts();
};


#endif