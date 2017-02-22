#include "RoutinePhysics.hh"


#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4SynchrotronRadiation.hh"
#include "G4SynchrotronRadiationInMat.hh"

#include "G4StepLimiter.hh"

#include "G4SystemOfUnits.hh"
#include "G4AutoDelete.hh"

//******************************
// QBBC
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineQBBC::RoutineQBBC(G4int ver)
{
    name = "QBBC";
    defaultCutValue = 0.7 * CLHEP::mm;
    SetVerboseLevel(ver);
    RegisterPhysics( new G4EmStandardPhysics(ver) );
    RegisterPhysics( new G4EmExtraPhysics(ver) );
    RegisterPhysics( new G4DecayPhysics(ver) );
    RegisterPhysics( new G4HadronElasticPhysicsXS(ver) );
    RegisterPhysics( new G4StoppingPhysics(ver) );
    RegisterPhysics( new G4IonPhysics(ver) );
    RegisterPhysics( new G4HadronInelasticQBBC(ver));
    RegisterPhysics( new G4NeutronTrackingCut(ver) );
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineQBBC::~RoutineQBBC()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineQBBC::SetCuts()
{
    SetCutsWithDefault();
}

//******************************
// Topas
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineTopas::RoutineTopas(G4int ver) : RoutineModularPhysics()
{
    name = "topas";

    defaultCutValue = 0.7 * CLHEP::mm;
    SetVerboseLevel(ver);

    RegisterPhysics( new G4EmStandardPhysics_option4(ver) );
    RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(ver));
    RegisterPhysics( new G4DecayPhysics(ver) );
    RegisterPhysics( new G4IonBinaryCascadePhysics(ver));
    RegisterPhysics( new G4HadronElasticPhysicsHP(ver) );
    RegisterPhysics( new G4StoppingPhysics(ver) );
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineTopas::~RoutineTopas()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineTopas::SetCuts()
{
    SetCutsWithDefault();
}

//******************************
// minimalist
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineMinimalist::RoutineMinimalist(G4int ver) : RoutineUserPhysics()
{
    name = "minimalist";
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineMinimalist::~RoutineMinimalist()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMinimalist::SetCuts()
{
    // defaultCutValue = 0.7 * CLHEP::mm;
    // SetCutValue(defaultCutValue, "gamma");

    // prevent secondary electrons
    SetCutValue(1.0 * mm, "proton");
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMinimalist::ConstructParticle()
{
    G4Proton::Proton();
    G4Gamma::Gamma();
    G4Electron::Electron();
    G4Positron::Positron();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMinimalist::ConstructProcess()
{
    AddTransportation();

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    theParticleIterator->reset();
    while((*theParticleIterator)())
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4String particleName = particle->GetParticleName();

        G4cout << "    ConstructProcess() for " << particleName << G4endl;

        if (particleName == "proton")
        {
            ph->RegisterProcess(new G4hIonisation(), particle);

            // G4BinaryCascade* model = new G4BinaryCascade();
            // model->SetMinEnergy(0);
            // model->SetMaxEnergy(9.9 * GeV);
            // G4ProtonInelasticProcess* proc = new G4ProtonInelasticProcess();
            // proc->RegisterMe(model);
            // ph->RegisterProcess(proc, particle);
        }

        // if (particleName == "gamma")
        // {
            // ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
            // ph->RegisterProcess(new G4ComptonScattering(), particle);
            // ph->RegisterProcess(new G4GammaConversion(), particle);
        // }

    }
}



