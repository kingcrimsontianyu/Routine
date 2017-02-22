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
        G4EventManager* em = G4EventManager::GetEventManager();
        G4int eventId = em->GetConstCurrentEvent()->GetEventID();

        const G4ParticleDefinition* pd = track->GetDefinition();
        G4String particleName = pd->GetParticleName();
        G4int trackId = track->GetTrackID();

        // primary particle
        if(trackId == 1)
        {
            G4cout << "--> primary particle = " << particleName << ", event id = " << eventId << G4endl;
        }
        // secondary particle
        else
        {
            G4cout << "    secondary particle = " << std::setw(12) << particleName
                   << ", track id = " << std::setw(4) << trackId
                   << ", parent track id = " << std::setw(4) << track->GetParentID()
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
    // loop through particles
    while((*it)())
    {
        G4ParticleDefinition* particle = it->value();
        G4cout << "--> " << particle->GetParticleName() << G4endl;
        supportedParticleList.push_back(particle->GetParticleName());

        G4ProcessManager* pm = particle->GetProcessManager();
        G4ProcessVector* pv = pm->GetProcessList();

        // loop through process name
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
void RoutineUtility::AccumulateCount(const G4Track* track)
{
    PrintParticleInfo(track);

    const G4ParticleDefinition* pd = track->GetDefinition();
    G4String particleName = pd->GetParticleName();
    G4AnalysisManager* analysisManager = GetAnalysisManager();
    auto h1Size = analysisManager->EndConstH1() - analysisManager->BeginConstH1();
    for(G4int i = 0; i < h1Size; ++i)
    {
        G4String h1Name = analysisManager->GetH1Name(i);
        G4String temp = particleName + "_count";
        if(h1Name == temp)
        {
            analysisManager->FillH1(i, 1.0);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::AccumulateSpectrum(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    const G4ParticleDefinition* pd = track->GetDefinition();
    G4String particleName = pd->GetParticleName();
    G4AnalysisManager* analysisManager = GetAnalysisManager();
    auto h1Size = analysisManager->EndConstH1() - analysisManager->BeginConstH1();
    for(G4int i = 0; i < h1Size; ++i)
    {
        G4String h1Name = analysisManager->GetH1Name(i);
        G4String temp = particleName + G4String("_spectrum");
        if(h1Name == temp)
        {
            analysisManager->FillH1(i, step->GetTotalEnergyDeposit());
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
// sort according to value of pair<key,value> in descending order
bool SortFunc(std::pair<G4String, double> i, std::pair<G4String, double> j)
{
    return (i.second > j.second);
}

// find the first occurrence of zero value
bool FindFunc(std::pair<G4String, double> i)
{
    return (i.second == 0.0);
}

bool IsInParticleList(G4String& particle, // return particle name, for example: proton
                      const G4String& p,  // string name, for example: proton_count
                      const std::vector<G4String>& supportedParticleList,
                      const G4String& suffix = "")
{
    for(G4int i = 0; i < supportedParticleList.size(); ++i)
    {
        G4String temp = supportedParticleList[i] + suffix;
        if(p == temp)
        {
            particle = supportedParticleList[i];
            return true;
        }
    }
    particle = "";
    return false;
}

void RoutineUtility::SaveHistToFile()
{
    std::ofstream file("histogram.txt");

    if(file.is_open())
    {
        #if defined G4MULTITHREADED
        G4RunManager* rm = G4RunManager::GetRunManager();
        #else
        G4MTRunManager* rm = G4MTRunManager::GetMasterRunManager();
        #endif
        G4int numHistory = rm->GetNumberOfEventsToBeProcessed();
        file << "--> total number of primary particles = " << numHistory << G4endl;

        auto detectorConstruction = static_cast<const RoutineDetectorConstruction*>(rm->GetUserDetectorConstruction());
        G4double mass = detectorConstruction->GetPhantomMass();

        //++++++++++++++++++++++++++++++
        // ordered particle count
        //++++++++++++++++++++++++++++++
        G4AnalysisManager* analysisManager = GetAnalysisManager();
        auto h1Size = analysisManager->EndConstH1() - analysisManager->BeginConstH1();
        std::vector<std::pair<G4String, double>> tempVec;
        for(G4int i = 0; i < h1Size; ++i)
        {
            G4String h1Name = analysisManager->GetH1Name(i);
            G4String particleName;
            // among supported particles, retain histogram entries with the name of ``particleName_count''
            if(IsInParticleList(particleName, h1Name, supportedParticleList, "_count"))
            {
                tools::histo::h1d* h1 = analysisManager->GetH1(i);
                double count = h1->sum_bin_heights();
                tempVec.push_back(std::make_pair(particleName, count));

                // log active particles
                if(count != 0.0)
                {
                    activeParticleList.push_back(particleName);
                }
            }
        }

        // sort the vector
        std::sort(tempVec.begin(), tempVec.end(), SortFunc);
        auto it = std::find_if(tempVec.begin(), tempVec.end(), FindFunc);

        // write
        file << G4endl << "--> total number of particles supported = " << tempVec.size() << G4endl;
        auto num = it - tempVec.begin();
        file << "    number of particles actually simulated = " << num << G4endl << G4endl;
        file << "--> count (per source particle)" << G4endl;

        for(G4int i = 0; i < tempVec.size(); ++i)
        {
            file << "    " << std::setw(30) << std::left << tempVec[i].first
                 << std::setw(15) << std::scientific << tempVec[i].second / numHistory << G4endl;

            if(i == num - 1)
            {
                file << G4endl;
            }
        }

        //++++++++++++++++++++++++++++++
        // spectrum
        //++++++++++++++++++++++++++++++
        file << G4endl << "--> dose spectrum (per source particle) [MeV/g/par]" << G4endl;
        G4double sum = 0.0;
        // retain histogram entries with the name of ``particleName_spectrum''
        for(G4int i = 0; i < h1Size; ++i)
        {
            G4String h1Name = analysisManager->GetH1Name(i);
            G4String particleName;
            // among supported particles, retain histogram entries with the name of ``particleName_count''
            if(IsInParticleList(particleName, h1Name, supportedParticleList, "_spectrum"))
            {
                // among active particles, retain histogram entries with the name of ``particleName_count''
                if(IsInParticleList(particleName, h1Name, activeParticleList, "_spectrum"))
                {
                    tools::histo::h1d* h1 = analysisManager->GetH1(i);
                    double dose = h1->sum_bin_heights() / mass / (MeV / g) / numHistory;
                    sum += dose;
                    file << "    " << std::setw(30) << std::left << particleName
                         << std::setw(15) << std::scientific << dose << G4endl;
                }
            }
        }
        file << "    " << std::setw(30) << std::left << "sum"
                       << std::setw(15) << std::scientific << sum << G4endl;

        file.close();
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
    G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* it = theParticleTable->GetIterator();
    it->reset();
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // loop through all supported particles
    while((*it)())
    {
        G4ParticleDefinition* particle = it->value();
        G4int ih = analysisManager->CreateH1(particle->GetParticleName() + "_count", particle->GetParticleName() + " count", 1, 0.0, 200.0 * MeV);
        analysisManager->SetH1Ascii(ih, true);

        ih = analysisManager->CreateH1(particle->GetParticleName() + "_spectrum", particle->GetParticleName() + " spectrum", 1, 0.0, 200.0 * MeV);
        analysisManager->SetH1Ascii(ih, true);
    }
}




