#ifndef ROUTINE_PHYSICSLIST_H
#define ROUTINE_PHYSICSLIST_H

#include "G4VModularPhysicsList.hh"

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
// QBBC
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
class RoutineQBBC: public RoutineModularPhysics
{
public:
    RoutineQBBC(G4int ver = 0);
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
    RoutineTopas(G4int ver = 0);
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
    RoutineMiniProton(G4int ver = 0);
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
    RoutineMiniGamma(G4int ver = 0);
    virtual ~RoutineMiniGamma();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    virtual void SetCuts();
};



#endif