#include "RoutineRunAction.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineRunAction::RoutineRunAction(RoutineParameterManager* rp_ext, RoutineUtility* rut_ext) :
G4UserRunAction(), fEdep("Edep", 0.0), fEdep2("Edep2", 0.0), rp(rp_ext), rut(rut_ext)
{
    // Register parameter to the parameter manager
    G4AccumulableManager* parameterManager = G4AccumulableManager::Instance();
    parameterManager->RegisterAccumulable(fEdep);
    parameterManager->RegisterAccumulable(fEdep2);
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineRunAction::~RoutineRunAction()
{}

//------------------------------------------------------------
//------------------------------------------------------------
G4Run* RoutineRunAction::GenerateRun()
{
    return new RoutineRun(G4String("PhantomMFD"));
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRunAction::BeginOfRunAction(const G4Run*)
{
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    // reset parameters to their initial values
    G4AccumulableManager* parameterManager = G4AccumulableManager::Instance();
    parameterManager->Reset();
}

//------------------------------------------------------------
// after all particles have been simulated
//------------------------------------------------------------
void RoutineRunAction::EndOfRunAction(const G4Run* run)
{
    G4int numHistory = run->GetNumberOfEvent();

    if (numHistory == 0) return;
    // Merge parameters
    G4AccumulableManager* parameterManager = G4AccumulableManager::Instance();
    parameterManager->Merge();

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // dose based on stepping
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(IsMaster())
    {
        G4cout << "--> Output absorbed dose based on stepping." << G4endl;

        // convert energy to dose per source particle
        auto detectorConstruction = static_cast<const RoutineDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        G4double mass = detectorConstruction->GetPhantomMass();

        G4double edep  = fEdep.GetValue();
        G4double edep2 = fEdep2.GetValue();

        G4double dose  = edep / mass;
        G4double dose2 = edep2 / mass / mass;
        G4double sigma = dose2 - dose * dose / numHistory;
        if (sigma > 0.0)
        {
            sigma = std::sqrt(sigma);
        }
        else
        {
            sigma = 0.0;
        }
        dose /= numHistory;
        sigma /= numHistory;

        G4double rsd;
        if(dose != 0.0)
        {
            rsd = sigma / dose;
        }
        else
        {
            rsd = 0.0;
        }

        G4cout << "    history = " << run->GetNumberOfEvent() << G4endl;
        G4cout << "    dose = " << std::setprecision(15) << dose / (MeV / g) << " [MeV/g]"<< G4endl;
        G4cout << "    rsd = " << rsd * 100.0 << " [%]" << G4endl;
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // dose
    // scoring based on hits map (voxel dose)
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(IsMaster())
    {
        G4cout << "--> Output absorbed dose based on hits map (voxel dose)." << G4endl;

        // convert energy to dose per source particle
        auto detectorConstruction = static_cast<const RoutineDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        G4double mass = detectorConstruction->GetPhantomMass();

        auto biuRun = static_cast<const RoutineRun*>(run);
        G4THitsMap<G4double>* hitsMap = biuRun->GetHitsMap(G4String("PhantomMFD/energyImparted3D"));
        G4THitsMap<G4double>* hitsMapSquared = biuRun->GetHitsMapSquared(G4String("PhantomMFD/energyImparted3DSquared"));

        G4double edep = 0.0;
        for(auto it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); ++it)
        {
            edep += *(it->second);
        }
        G4double dose  = edep / mass;
        dose /= numHistory;
        G4cout << "    dose = " << dose / (MeV / g) << " [MeV/g]"<< G4endl;

        // save result to file
        // get phantom parameters
        RoutineThreeVector<G4int> numVoxel = detectorConstruction->GetNumVoxel();
        G4double voxelVolume = detectorConstruction->GetVoxelVolume();
        G4cout << "    voxel volume = " << voxelVolume / cm3 << G4endl;

        G4String path = "dose_" + rp->param->outputSuffix + ".txt";
        G4cout << "    save to file " << path << G4endl;
        std::ofstream file(path);
        // store data in column major where x index changes fastest
        for(G4int k = 0; k < numVoxel.z; ++k)
        {
            for(G4int j = 0; j < numVoxel.y; ++j)
            {
                for(G4int i = 0; i < numVoxel.x; ++i)
                {
                    G4double edep_temp = 0.0;
                    G4double edep2_temp = 0.0;

                    // x idx changes fastest, z slowest
                    G4int globalIdx = numVoxel.x * numVoxel.y * k + numVoxel.x * j + i;
                    G4double* edepPtr = (*hitsMap)[globalIdx];
                    if(edepPtr != nullptr)
                    {
                        edep_temp = *edepPtr;
                    }
                    G4double* edep2Ptr = (*hitsMapSquared)[globalIdx];
                    if(edep2Ptr != nullptr)
                    {
                        edep2_temp = *edep2Ptr;
                    }

                    // convert from energy to dose
                    G4Material* material = detectorConstruction->GetPhantomMaterial(globalIdx);
                    G4double density = material->GetDensity();
                    G4double mass_temp = density * voxelVolume;
                    G4double dose_temp  = edep_temp / mass_temp;
                    G4double dose2_temp = edep2_temp / mass_temp / mass_temp;

                    G4double sigma = dose2_temp - dose_temp * dose_temp / numHistory;
                    if (sigma > 0.0)
                    {
                        sigma = std::sqrt(sigma);
                    }
                    else
                    {
                        sigma = 0.0;
                    }
                    dose_temp /= numHistory;
                    sigma /= numHistory;
                    G4double rsd;
                    if(dose_temp != 0.0)
                    {
                        rsd = sigma / dose_temp;
                    }
                    else
                    {
                        rsd = 0.0;
                    }

                    file << std::setw(5) << i
                         << std::setw(5) << j
                         << std::setw(5) << k
                         << std::setw(20) << std::scientific << std::setprecision(12) << dose_temp / (MeV / g)
                         << std::setw(20) << std::scientific << std::setprecision(12) << rsd << G4endl;

                }
            }
        }
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // kerma
    // scoring based on hits map (voxel dose)
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(IsMaster())
    {
        G4cout << "--> Output kerma based on hits map (voxel dose)." << G4endl;

        // convert energy to dose per source particle
        auto detectorConstruction = static_cast<const RoutineDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        G4double mass = detectorConstruction->GetPhantomMass();

        auto biuRun = static_cast<const RoutineRun*>(run);
        G4THitsMap<G4double>* hitsMap = biuRun->GetHitsMap(G4String("PhantomMFD/energyTransfer3D"));
        G4THitsMap<G4double>* hitsMapSquared = biuRun->GetHitsMapSquared(G4String("PhantomMFD/energyTransfer3DSquared"));

        G4double edep = 0.0;
        for(auto it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); ++it)
        {
            edep += *(it->second);
        }
        G4double dose  = edep / mass;
        dose /= numHistory;
        G4cout << "    kerma = " << dose / (MeV / g) << " [MeV/g]"<< G4endl;

        // save result to file
        // get phantom parameters
        RoutineThreeVector<G4int> numVoxel = detectorConstruction->GetNumVoxel();
        G4double voxelVolume = detectorConstruction->GetVoxelVolume();
        G4cout << "    voxel volume = " << voxelVolume / cm3 << G4endl;

        G4String path = "kerma_" + rp->param->outputSuffix + ".txt";
        G4cout << "    save to file " << path << G4endl;
        std::ofstream file(path);
        // store data in column major where x index changes fastest
        for(G4int k = 0; k < numVoxel.z; ++k)
        {
            for(G4int j = 0; j < numVoxel.y; ++j)
            {
                for(G4int i = 0; i < numVoxel.x; ++i)
                {
                    G4double edep_temp = 0.0;
                    G4double edep2_temp = 0.0;

                    // x idx changes fastest, z slowest
                    G4int globalIdx = numVoxel.x * numVoxel.y * k + numVoxel.x * j + i;
                    G4double* edepPtr = (*hitsMap)[globalIdx];
                    if(edepPtr != nullptr)
                    {
                        edep_temp = *edepPtr;
                    }
                    G4double* edep2Ptr = (*hitsMapSquared)[globalIdx];
                    if(edep2Ptr != nullptr)
                    {
                        edep2_temp = *edep2Ptr;
                    }

                    // convert from energy to dose
                    G4Material* material = detectorConstruction->GetPhantomMaterial(globalIdx);
                    // G4cout << material->GetName() << G4endl;
                    G4double density = material->GetDensity();
                    G4double mass_temp = density * voxelVolume;
                    G4double dose_temp  = edep_temp / mass_temp;
                    G4double dose2_temp = edep2_temp / mass_temp / mass_temp;

                    G4double sigma = dose2_temp - dose_temp * dose_temp / numHistory;
                    if (sigma > 0.0)
                    {
                        sigma = std::sqrt(sigma);
                    }
                    else
                    {
                        sigma = 0.0;
                    }
                    dose_temp /= numHistory;
                    sigma /= numHistory;
                    G4double rsd;
                    if(dose_temp != 0.0)
                    {
                        rsd = sigma / dose_temp;
                    }
                    else
                    {
                        rsd = 0.0;
                    }

                    file << std::setw(5) << i
                         << std::setw(5) << j
                         << std::setw(5) << k
                         << std::setw(20) << std::scientific << std::setprecision(12) << dose_temp / (MeV / g)
                         << std::setw(20) << std::scientific << std::setprecision(12) << rsd << G4endl;

                }
            }
        }
    }

    // histograms
    if(IsMaster())
    {
        G4cout << "--> Output custom scores." << G4endl;
        rut->SaveCustomScoreToFile();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRunAction::AddEdep(G4double edep)
{
    fEdep  += edep;
    fEdep2 += edep * edep;
}




