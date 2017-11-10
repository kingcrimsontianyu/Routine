#ifndef ROUTINE_PHYSICSLIST_H
#define ROUTINE_PHYSICSLIST_H

#include "G4VModularPhysicsList.hh"
#include "RoutineParameter.hh"

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineModularPhysics : public G4VModularPhysicsList
{
public:
    G4String name;
    RoutineModularPhysics(RoutineParameterManager* rp);
    virtual ~RoutineModularPhysics();
protected:
    RoutineParameterManager* rp;
};

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineUserPhysics : public G4VUserPhysicsList
{
public:
    G4String name;
    RoutineUserPhysics(RoutineParameterManager* rp);
    virtual ~RoutineUserPhysics();
protected:
    RoutineParameterManager* rp;
};

//******************************
// QBBC
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineQBBC: public RoutineModularPhysics
{
public:
    RoutineQBBC(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineQBBC();
    virtual void SetCuts();
};

//******************************
// Topas
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineTopas: public RoutineModularPhysics
{
public:
    RoutineTopas(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineTopas();
    virtual void SetCuts();
};

//******************************
// tweaker
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineTweaker: public RoutineModularPhysics
{
public:
    RoutineTweaker(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineTweaker();
    virtual void SetCuts();
};

//******************************
// mini proton
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineMiniProton: public RoutineUserPhysics
{
public:
    RoutineMiniProton(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineMiniProton();
    virtual void ConstructParticle();
    virtual void ConstructProcess();

protected:
    G4VPhysicsConstructor* fHadronPhys;
};

//******************************
// mini gamma
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineMiniGamma: public RoutineUserPhysics
{
public:
    RoutineMiniGamma(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineMiniGamma();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    virtual void SetCuts();
};

//******************************
// multiple scattering proton
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineMultipleScatteringProton: public RoutineUserPhysics
{
public:
    RoutineMultipleScatteringProton(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineMultipleScatteringProton();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
};

//******************************
// single scattering proton
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineSingleScatteringProton: public RoutineUserPhysics
{
public:
    RoutineSingleScatteringProton(RoutineParameterManager* rp, G4int ver = 0);
    virtual ~RoutineSingleScatteringProton();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
};

#endif