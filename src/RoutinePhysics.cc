#include "RoutinePhysics.hh"

#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

#include "G4ePairProduction.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4Generator2BS.hh"
#include "G4Generator2BN.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4UrbanMscModel.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4UAtomicDeexcitation.hh"

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

#include "globals.hh"
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
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4hIonisation.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4LossTableManager.hh"

#include "G4HadronElasticProcess.hh"
#include "G4HadronElastic.hh"
#include "G4CrossSectionDataSetRegistry.hh"
#include "G4ChipsProtonElasticXS.hh"
#include "G4ChipsElasticModel.hh"
#include "G4EmProcessOptions.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineModularPhysics::RoutineModularPhysics(RoutineParameterManager* rp_ext)
: G4VModularPhysicsList(),
rp(rp_ext)
{}

RoutineModularPhysics::~RoutineModularPhysics()
{}


//------------------------------------------------------------
//------------------------------------------------------------
RoutineUserPhysics::RoutineUserPhysics(RoutineParameterManager* rp_ext)
: G4VUserPhysicsList(),
rp(rp_ext)
{}

RoutineUserPhysics::~RoutineUserPhysics()
{}

//******************************
// QBBC
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineQBBC::RoutineQBBC(RoutineParameterManager* rp_ext, G4int ver) :
RoutineModularPhysics(rp_ext)
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
RoutineTopas::RoutineTopas(RoutineParameterManager* rp_ext, G4int ver) :
RoutineModularPhysics(rp_ext)
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
// mini proton
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineMiniProton::RoutineMiniProton(RoutineParameterManager* rp_ext, G4int) :
RoutineUserPhysics(rp_ext)
{
    name = "mini-proton";
    defaultCutValue = 1000.0 * cm;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineMiniProton::~RoutineMiniProton()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMiniProton::ConstructParticle()
{
    G4Proton::Proton();
    G4Gamma::Gamma();
    G4Electron::Electron();
    G4Positron::Positron();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMiniProton::ConstructProcess()
{
    AddTransportation();

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    GetParticleIterator()->reset();
    while((*GetParticleIterator())())
    {
        G4ParticleDefinition* particle = GetParticleIterator()->value();
        G4String particleName = particle->GetParticleName();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        G4cout << "    ConstructProcess() for " << particleName << G4endl;

        if(particleName == "proton")
        {
            // ionization
            auto hIon = new G4hIonisation();
            if(rp->param->disableFluctuation)
            {
                hIon->SetLossFluctuations(false);
            }
            else
            {
                hIon->SetLossFluctuations(true);
            }
            ph->RegisterProcess(hIon, particle);

            // multiple scattering
            G4hMultipleScattering* pmsc = new G4hMultipleScattering();
            ph->RegisterProcess(pmsc, particle);

            // bremsstrahlung
            G4hBremsstrahlung* pb = new G4hBremsstrahlung();
            ph->RegisterProcess(pb, particle);

            // pair production
            G4hPairProduction* pp = new G4hPairProduction();
            ph->RegisterProcess(pp, particle);

            // coulomb scattering
            G4CoulombScattering* pss = new G4CoulombScattering();
            ph->RegisterProcess(pss, particle);

            // G4EmProcessOptions ctor calls singleton G4EmParameters
            // G4EmProcessOptions emOptions;
            // emOptions.SetPolarAngleLimit(0.0);

            // proton elastic
            // G4HadronElasticProcess* hel = new G4HadronElasticProcess();
            // G4ChipsElasticModel* chipsp = new G4ChipsElasticModel();
            // hel->AddDataSet(G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsProtonElasticXS::Default_Name()));
            // hel->RegisterMe(chipsp);
            // pmanager->AddDiscreteProcess(hel);

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

    // deexcitation
    // G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    // G4LossTableManager::Instance()->SetAtomDeexcitation(de);

    // //
    // fHadronPhys = new G4HadronElasticPhysics();
    // fHadronPhys->ConstructProcess();

}

//******************************
// mini gamma
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineMiniGamma::RoutineMiniGamma(RoutineParameterManager* rp_ext, G4int) :
RoutineUserPhysics(rp_ext)
{
    name = "mini-gamma";
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineMiniGamma::~RoutineMiniGamma()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMiniGamma::SetCuts()
{
    SetCutsWithDefault();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMiniGamma::ConstructParticle()
{
    G4Gamma::Gamma();
    G4Electron::Electron();
    G4Positron::Positron();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMiniGamma::ConstructProcess()
{
    AddTransportation();

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    GetParticleIterator()->reset();
    while((*GetParticleIterator())())
    {
        G4ParticleDefinition* particle = GetParticleIterator()->value();
        G4String particleName = particle->GetParticleName();

        G4cout << "    ConstructProcess() for " << particleName << G4endl;

        // energy limits for e+- scattering models
        G4double highEnergyLimit = 100 * MeV;

        if (particleName == "gamma")
        {
            ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
            ph->RegisterProcess(new G4ComptonScattering(), particle);
            ph->RegisterProcess(new G4GammaConversion(), particle);
            ph->RegisterProcess(new G4RayleighScattering(), particle);
        }
        else if (particleName == "e-")
        {
            // from em standard op1
            G4eIonisation* eioni = new G4eIonisation();
            eioni->SetStepFunction(0.8, 1.0*mm);

            G4eMultipleScattering* msc = new G4eMultipleScattering;
            G4UrbanMscModel* msc1 = new G4UrbanMscModel();
            G4WentzelVIModel* msc2 = new G4WentzelVIModel();
            msc1->SetNewDisplacementFlag(false);
            msc1->SetHighEnergyLimit(highEnergyLimit);
            msc2->SetLowEnergyLimit(highEnergyLimit);
            msc->AddEmModel(0, msc1);
            msc->AddEmModel(0, msc2);

            G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();
            G4CoulombScattering* ss = new G4CoulombScattering();
            ss->SetEmModel(ssm, 1);
            ss->SetMinKinEnergy(highEnergyLimit);
            ssm->SetLowEnergyLimit(highEnergyLimit);
            ssm->SetActivationLowEnergyLimit(highEnergyLimit);

            ph->RegisterProcess(msc, particle);
            ph->RegisterProcess(eioni, particle);
            ph->RegisterProcess(new G4eBremsstrahlung(), particle);
            ph->RegisterProcess(ss, particle);
        }
        else if (particleName == "e+")
        {
            // from em standard op1
            G4eIonisation* eioni = new G4eIonisation();
            eioni->SetStepFunction(0.8, 1.0*mm);

            G4eMultipleScattering* msc = new G4eMultipleScattering;
            G4UrbanMscModel* msc1 = new G4UrbanMscModel();
            G4WentzelVIModel* msc2 = new G4WentzelVIModel();
            msc1->SetNewDisplacementFlag(false);
            msc1->SetHighEnergyLimit(highEnergyLimit);
            msc2->SetLowEnergyLimit(highEnergyLimit);
            msc->AddEmModel(0, msc1);
            msc->AddEmModel(0, msc2);

            G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();
            G4CoulombScattering* ss = new G4CoulombScattering();
            ss->SetEmModel(ssm, 1);
            ss->SetMinKinEnergy(highEnergyLimit);
            ssm->SetLowEnergyLimit(highEnergyLimit);
            ssm->SetActivationLowEnergyLimit(highEnergyLimit);

            ph->RegisterProcess(msc, particle);
            ph->RegisterProcess(eioni, particle);
            ph->RegisterProcess(new G4eBremsstrahlung(), particle);
            ph->RegisterProcess(new G4eplusAnnihilation(), particle);
            ph->RegisterProcess(ss, particle);
        }

    }
}






//******************************
// single scattering proton
//******************************
//------------------------------------------------------------
//------------------------------------------------------------
RoutineSingleScatteringProton::RoutineSingleScatteringProton(RoutineParameterManager* rp_ext, G4int) :
RoutineUserPhysics(rp_ext)
{
    name = "single-scattering-proton";
    defaultCutValue = 1000.0 * cm;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineSingleScatteringProton::~RoutineSingleScatteringProton()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineSingleScatteringProton::ConstructParticle()
{
    G4Proton::Proton();
    G4Gamma::Gamma();
    G4Electron::Electron();
    G4Positron::Positron();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineSingleScatteringProton::ConstructProcess()
{
    AddTransportation();

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    GetParticleIterator()->reset();
    while((*GetParticleIterator())())
    {
        G4ParticleDefinition* particle = GetParticleIterator()->value();
        G4String particleName = particle->GetParticleName();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        G4cout << "    ConstructProcess() for " << particleName << G4endl;

        if(particleName == "proton")
        {
            // ionization
            auto hIon = new G4hIonisation();
            if(rp->param->disableFluctuation)
            {
                hIon->SetLossFluctuations(false);
            }
            else
            {
                hIon->SetLossFluctuations(true);
            }
            ph->RegisterProcess(hIon, particle);

            // bremsstrahlung
            G4hBremsstrahlung* pb = new G4hBremsstrahlung();
            ph->RegisterProcess(pb, particle);

            // pair production
            G4hPairProduction* pp = new G4hPairProduction();
            ph->RegisterProcess(pp, particle);

            // coulomb scattering
            G4CoulombScattering* pss = new G4CoulombScattering();
            ph->RegisterProcess(pss, particle);

            auto theParameters = G4EmParameters::Instance();
            theParameters->SetMscThetaLimit(0.0);

            // proton elastic
            // G4HadronElasticProcess* hel = new G4HadronElasticProcess();
            // G4ChipsElasticModel* chipsp = new G4ChipsElasticModel();
            // hel->AddDataSet(G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsProtonElasticXS::Default_Name()));
            // hel->RegisterMe(chipsp);
            // pmanager->AddDiscreteProcess(hel);

            // G4BinaryCascade* model = new G4BinaryCascade();
            // model->SetMinEnergy(0);
            // model->SetMaxEnergy(9.9 * GeV);
            // G4ProtonInelasticProcess* proc = new G4ProtonInelasticProcess();
            // proc->RegisterMe(model);
            // ph->RegisterProcess(proc, particle);
        }
    }

    // deexcitation
    // G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    // G4LossTableManager::Instance()->SetAtomDeexcitation(de);

    // //
    // fHadronPhys = new G4HadronElasticPhysics();
    // fHadronPhys->ConstructProcess();
}
