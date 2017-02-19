#include "RoutineUtility.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineUtility::RoutineUtility() : fHistManager(nullptr), bPrintParticleInfo(false)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineUtility::~RoutineUtility()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::SetPrintParticleInfo(G4bool choose)
{
    bPrintParticleInfo = choose;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::PrintParticleInfo(const G4Track* track)
{
    if(bPrintParticleInfo)
    {
        const G4ParticleDefinition* pd = track->GetDefinition();
        G4String particleName = pd->GetParticleName();
        G4int id = track->GetTrackID();

        // primary particle
        if(id == 1)
        {
            G4cout << "--> primary particle = " << particleName << G4endl;
        }
        // secondary particle
        else
        {
            G4cout << "    secondary particle = " << particleName
                   << " self id = " << id
                   << " parent id = "
                   << track->GetParentID()
                   << G4endl;
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::PrintPhysicsInfo()
{
    G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* it = theParticleTable->GetIterator();
    it->reset();
    while((*it)())
    {
        G4ParticleDefinition* particle = it->value();
        G4cout << "--> " << particle->GetParticleName() << G4endl;

        G4ProcessManager* pm = particle->GetProcessManager();
        G4ProcessVector* pv = pm->GetProcessList();

        for(G4int i = 0; i < pv->size(); ++i)
        {
            G4VProcess* proc = (*pv)[i];
            G4cout << "    " << proc->GetProcessName() << G4endl;
        }

    }
    G4cout << G4endl;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4HistManager* RoutineUtility::CreateHistManager()
{
    fHistManager = new G4HistManager();
    return fHistManager;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::DeleteHistManager()
{
    delete fHistManager; fHistManager = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::AccumulateCounter(const G4Track* track)
{
    PrintParticleInfo(track);

    const G4ParticleDefinition* pd = track->GetDefinition();
    G4String particleName = pd->GetParticleName();
    G4AnalysisManager* analysisManager = GetAnalysisManager();
    auto h1Size = analysisManager->EndConstH1() - analysisManager->BeginConstH1();
    for(G4int i = 0; i < h1Size; ++i)
    {
        G4String h1Name = analysisManager->GetH1Name(i);
        if(h1Name == particleName)
        {
            analysisManager->FillH1(i, 1.0);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::PrintVisualInfo()
{
    G4VisManager* vm = G4VisManager::GetInstance();
    auto gph = vm->GetAvailableGraphicsSystems();
    G4cout << "--> available graphics systems:" << G4endl;
    for(size_t i = 0; i < gph.size(); ++i)
    {
        G4cout << "    " << gph[i]->GetName() << G4endl;
    }

    G4cout << "    current graphics system:" << G4endl;
    if(vm->GetCurrentGraphicsSystem() == nullptr)
    {
        G4cout << "    none." << G4endl;
    }
    else
    {
        G4cout << "    " << vm->GetCurrentGraphicsSystem()->GetName() << G4endl;
    }
}





//------------------------------------------------------------
//------------------------------------------------------------
G4AnalysisManager* RoutineUtility::GetAnalysisManager()
{
    return G4AnalysisManager::Instance();
}

//------------------------------------------------------------
//------------------------------------------------------------
G4HistManager* RoutineUtility::GetHistManager()
{
    return fHistManager;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4HistManager::G4HistManager() : fFileName("histogram")
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetFileName(fFileName);
    // analysisManager->SetVerboseLevel(1);
    // analysisManager->SetActivation(true);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4HistManager::~G4HistManager()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void G4HistManager::SetUpHist()
{
    G4int nbins = 1;
    G4double vmin = 0.0;
    G4double vmax = 200.0 * MeV;
    G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* it = theParticleTable->GetIterator();
    it->reset();
    G4int count = 0;
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    while((*it)())
    {
        G4ParticleDefinition* particle = it->value();
        G4int ih = analysisManager->CreateH1(particle->GetParticleName(), particle->GetParticleName() + " title", nbins, vmin, vmax);

        // analysisManager->SetH1Activation(ih, false);

        analysisManager->SetH1Ascii(ih, true);
        ++count;
    }
}



