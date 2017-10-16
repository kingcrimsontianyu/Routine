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



#endif