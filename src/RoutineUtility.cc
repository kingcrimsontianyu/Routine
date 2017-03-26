#include "RoutineUtility.hh"
#include "RoutineRun.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineUtility::RoutineUtility(RoutineParameterManager* rp) : bPrintParticleInfo(false), rp(rp)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineUtility::RoutineUtility()
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
void RoutineUtility::AccumulateCount(const G4Track* track)
{
    PrintParticleInfo(track);

    const G4ParticleDefinition* pd = track->GetDefinition();
    G4String particleName = pd->GetParticleName();

    #if defined G4MULTITHREADED
    G4RunManager* rm = G4RunManager::GetRunManager();
    #else
    G4MTRunManager* rm = G4MTRunManager::GetMasterRunManager();
    #endif

    G4Run* run = rm->GetNonConstCurrentRun();
    RoutineRun* localRun = static_cast<RoutineRun*>(run);

    // accumulate custom score
    if(localRun->GetCSMap().find(particleName) != localRun->GetCSMap().end())
    {
        localRun->GetCSMap()[particleName].AccumulateCount(1.0);
    }
    else // if the particle appears the first time
    {
        RoutineCustomScore temp(1.0, 0.0);
        localRun->GetCSMap().insert(std::pair<G4String, RoutineCustomScore>(particleName, temp));

        auto& ref = localRun->GetCSMap()[particleName].processList;
        G4ProcessManager* pm = pd->GetProcessManager();
        G4ProcessVector* pv = pm->GetProcessList();
        // loop through process name
        for(G4int i = 0; i < pv->size(); ++i)
        {
            G4VProcess* proc = (*pv)[i];
            bool isActive = pm->GetProcessActivation(proc);
            if(!isActive)
            {
                G4cout << ">>> inactive process detected." << G4endl;
            }

            ProcessBlob temp;
            temp.processName = proc->GetProcessName();

            do
            {
                // em process
                G4VEmProcess* vEmProcess = dynamic_cast<G4VEmProcess*>(proc);
                if(vEmProcess)
                {
                    // geant4 10.02.p02 does not have NumberOfModels() yet.
                    const G4VEmModel* model = vEmProcess->GetCurrentModel();
                    temp.modelName.push_back(model->GetName());
                    break;
                }

                // em process
                G4VEnergyLossProcess* vEnergyLossProcess = dynamic_cast<G4VEnergyLossProcess*>(proc);
                if(vEnergyLossProcess)
                {
                    G4int num = vEnergyLossProcess->NumberOfModels();
                    for(G4int i = 0; i < num; ++i)
                    {
                        G4VEmModel* model = vEnergyLossProcess->GetModelByIndex(i);
                        temp.modelName.push_back(model->GetName());
                    }
                    break;
                }

                // em process
                G4VMultipleScattering* vMultipleScattering = dynamic_cast<G4VMultipleScattering*>(proc);
                if(vMultipleScattering)
                {
                    // geant4 10.02.p02 does not have NumberOfModels() or GetCurrentModel() yet.
                    G4VEmModel* model = vMultipleScattering->EmModel();
                    temp.modelName.push_back(model->GetName());
                    break;
                }

                // hadronic process
                G4HadronicProcess* hadronicProcess = dynamic_cast<G4HadronicProcess*>(proc);
                if(hadronicProcess)
                {
                    std::vector<G4HadronicInteraction*>& hardList = hadronicProcess->GetHadronicInteractionList();
                    G4int num = hardList.size();
                    for(G4int i = 0; i < num; ++i)
                    {
                        G4HadronicInteraction* model = hardList[i];
                        temp.modelName.push_back(model->GetModelName());
                    }
                    break;
                }


                // other process
                break;
            }
            while(true);




            ref.push_back(temp);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineUtility::AccumulateEnergy(const G4Step* step)
{
    #if defined G4MULTITHREADED
    G4RunManager* rm = G4RunManager::GetRunManager();
    #else
    G4MTRunManager* rm = G4MTRunManager::GetMasterRunManager();
    #endif

    G4Run* run = rm->GetNonConstCurrentRun();
    RoutineRun* localRun = static_cast<RoutineRun*>(run);

    G4Track* track = step->GetTrack();
    const G4ParticleDefinition* pd = track->GetDefinition();
    G4String particleName = pd->GetParticleName();

    // accumulate custom score
    if(localRun->GetCSMap().find(particleName) != localRun->GetCSMap().end())
    {
        localRun->GetCSMap()[particleName].AccumulateEnergy(step->GetTotalEnergyDeposit());
    }
    else
    {
        RoutineCustomScore temp(0.0, step->GetTotalEnergyDeposit());
        localRun->GetCSMap().insert(std::pair<G4String, RoutineCustomScore>(particleName, temp));
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
void RoutineUtility::SaveCustomScoreToFile()
{
    std::ofstream file("histogram_" + rp->param->outputSuffix + ".txt");

    if(file.is_open())
    {
        #if defined G4MULTITHREADED
        G4RunManager* rm = G4RunManager::GetRunManager();
        #else
        G4MTRunManager* rm = G4MTRunManager::GetMasterRunManager();
        #endif
        G4Run* run = rm->GetNonConstCurrentRun();
        RoutineRun* mergedRun = static_cast<RoutineRun*>(run);
        G4int numHistory = rm->GetNumberOfEventsToBeProcessed();
        file << "--> total number of primary particles = " << numHistory << G4endl;

        G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
        auto detectorConstruction = static_cast<const RoutineDetectorConstruction*>(rm->GetUserDetectorConstruction());
        G4double mass = detectorConstruction->GetPhantomMass();
        G4int numActivePar = mergedRun->GetCSMap().size();
        file << "    number of particles actually simulated = " << numActivePar << G4endl << G4endl;
        file << "--> " << std::setw(30) << std::left << "particle"
             << std::setw(25) << "count per source"
             << std::setw(25) << "dose per source"
             << std::setw(25) << "PDG encoding"
             << G4endl;

        // print count and dose
        G4double sum = 0.0;
        for(auto it = mergedRun->GetCSMap().begin(); it != mergedRun->GetCSMap().end(); ++it)
        {
            G4ParticleDefinition* theParticle = theParticleTable->FindParticle(it->first);
            G4int PDGEncoding = theParticle->GetPDGEncoding();
            G4double dose = it->second.energy / mass / (MeV / g) / numHistory;
            sum += dose;
            file << "    " << std::setw(30) << std::left << it->first
                 << std::setw(25) << std::setprecision(10) << std::scientific << it->second.count / numHistory
                 << std::setw(25) << std::setprecision(10) << std::scientific << dose
                 << std::setw(25) << PDGEncoding
                 << G4endl;
        }
        file << "    total dose = " << std::setprecision(10) << sum << G4endl;

        // print physics
        file << G4endl << "--> " << std::setw(30) << std::left << "process list" << G4endl;
        for(auto it = mergedRun->GetCSMap().begin(); it != mergedRun->GetCSMap().end(); ++it)
        {
            file << "    " << std::setw(30) << std::left << it->first << G4endl;
            for(G4int j = 0; j < it->second.processList.size(); ++j)
            {
                file << "        " << std::setw(30) << std::left << it->second.processList[j].processName << ": ";

                for(G4int m = 0; m < it->second.processList[j].modelName.size(); ++m)
                {
                    file << it->second.processList[j].modelName[m];
                    if(m != it->second.processList[j].modelName.size() - 1)
                    {
                        file << ", ";
                    }
                }

                file << G4endl;
            }
        }

        file.close();
    }
}





//------------------------------------------------------------
//------------------------------------------------------------
RoutineCustomScore::RoutineCustomScore(G4double count, G4double energy)
{
    this->energy = energy;
    this->count  = count;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineCustomScore::RoutineCustomScore() : energy(0.0), count(0.0)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineCustomScore::~RoutineCustomScore()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineCustomScore& RoutineCustomScore::operator += (const RoutineCustomScore& rhs)
{
    energy += rhs.energy;
    count  += rhs.count;
    return *this;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineCustomScore& RoutineCustomScore::operator = (const RoutineCustomScore& rhs)
{
    energy = rhs.energy;
    count  = rhs.count;
    processList = rhs.processList;
    return *this;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineCustomScore::AccumulateEnergy(const G4double energy)
{
    this->energy += energy;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineCustomScore::AccumulateCount(const G4double count)
{
    this->count += count;
}


