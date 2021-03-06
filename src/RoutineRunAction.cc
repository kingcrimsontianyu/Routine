#include "RoutineRunAction.hh"
#include "RoutineAnalysisManager.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineRunAction::RoutineRunAction(RoutineParameterManager* rp_ext, RoutineUtility* rut_ext) :
G4UserRunAction(), fEdep("Edep", 0.0), fEdep2("Edep2", 0.0), rp(rp_ext), rut(rut_ext)
{
    // Register parameter to the parameter manager
    G4AccumulableManager* parameterManager = G4AccumulableManager::Instance();
    parameterManager->RegisterAccumulable(fEdep);
    parameterManager->RegisterAccumulable(fEdep2);

    fRoutineAnalysisManager = new RoutineAnalysisManager();
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineRunAction::~RoutineRunAction()
{
    delete fRoutineAnalysisManager;
}

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

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if(analysisManager->IsActive())
    {
        G4String filename = rp->param->outputDir + "/" + rp->param->outputSuffix;
        analysisManager->OpenFile(filename);
    }
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
    // scoring based on hits map (voxel tally)
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(IsMaster())
    {
        G4cout << "--> Output absorbed dose based on hits map (voxel tally)." << G4endl;
        auto biuRun = static_cast<const RoutineRun*>(run);
        G4THitsMap<G4double>* hitsMap = biuRun->GetHitsMap(G4String("PhantomMFD/energyImparted3D"));
        G4THitsMap<G4double>* hitsMapSquared = biuRun->GetHitsMapSquared(G4String("PhantomMFD/energyImparted3DSquared"));
        OutputVoxelTally("dose_voxel", numHistory, hitsMap, hitsMapSquared);
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // kerma
    // scoring based on hits map (voxel tally)
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(IsMaster())
    {
        G4cout << "--> Output kerma based on hits map (voxel tally)." << G4endl;
        auto biuRun = static_cast<const RoutineRun*>(run);
        G4THitsMap<G4double>* hitsMap = biuRun->GetHitsMap(G4String("PhantomMFD/energyTransfer3D"));
        G4THitsMap<G4double>* hitsMapSquared = biuRun->GetHitsMapSquared(G4String("PhantomMFD/energyTransfer3DSquared"));
        OutputVoxelTally("kerma_voxel", numHistory, hitsMap, hitsMapSquared);
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // track-length
    // scoring based on hits map (voxel tally)
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(IsMaster())
    {
        G4cout << "--> Output flux based on hits map (voxel tally)." << G4endl;
        auto biuRun = static_cast<const RoutineRun*>(run);
        G4THitsMap<G4double>* hitsMap = biuRun->GetHitsMap(G4String("PhantomMFD/trackLength3D"));
        G4THitsMap<G4double>* hitsMapSquared = biuRun->GetHitsMapSquared(G4String("PhantomMFD/trackLength3DSquared"));
        G4bool isDividedByMass = false;
        OutputVoxelTally("flux_voxel", numHistory, hitsMap, hitsMapSquared, isDividedByMass);
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // range
    // scoring based on hits map
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // if(IsMaster())
    // {
        // G4cout << "--> Output range." << G4endl;
        // auto biuRun = static_cast<const RoutineRun*>(run);
        // G4double mean, rsd;
        // CalculateMeanAndRsd(mean,
                            // rsd,
                            // numHistory,
                            // biuRun->fTrueRange,
                            // biuRun->fTrueRange2);
        // G4cout << "    True range = " << std::setprecision(16) << mean / (cm) << " [cm]\n"
               // << "    Relative standard deviation = " << std::setprecision(3) << rsd * 100.0 << " [%]" << G4endl;

        // CalculateMeanAndRsd(mean,
                            // rsd,
                            // numHistory,
                            // biuRun->fProjectedRange,
                            // biuRun->fProjectedRange2);
        // G4cout << "    Projected range = " << std::setprecision(16) << mean / (cm) << " [cm]\n"
               // << "    Relative standard deviation = " << std::setprecision(3) << rsd * 100.0 << " [%]" << G4endl;
    // }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // shortest-step process
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // if(IsMaster())
    // {
        // G4cout << "--> Output shortest-step process." << G4endl;
        // auto biuRun = static_cast<const RoutineRun*>(run);
        // G4cout << "    " << std::setw(20) << std::left << "Process" << std::setw(20) << "Occurrence per primary particle [%]" << G4endl;
        // for(auto it = biuRun->fShortestStepProcList.begin(); it != biuRun->fShortestStepProcList.end(); ++it)
        // {
            // G4cout << "    " << std::setw(20) << std::left << it->first
                   // << std::setw(20) << static_cast<G4double>(it->second) / static_cast<G4double>(numHistory) * 100.0 << G4endl;
        // }
    // }

    // histograms
    if(IsMaster())
    {
        G4cout << "--> Output custom scores." << G4endl;
        rut->SaveCustomScoreToFileByMaster();
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // G4 histograms
    // executed by all threads
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // if(analysisManager->IsActive())
    // {
        // analysisManager->Write();
        // analysisManager->CloseFile();
    // }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // scatter angle
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // if(IsMaster())
    // {
        // G4cout << "--> Output scatter angle." << G4endl;
        // auto biuRun = static_cast<const RoutineRun*>(run);
        // G4double mean, rsd;
        // CalculateMeanAndRsd(mean,
                            // rsd,
                            // numHistory,
                            // biuRun->fPolarAngle,
                            // biuRun->fPolarAngle2);
        // G4cout << "    Polar angle cosine = " << std::setprecision(16) << mean / pi * 180.0 << "[degree]\n"
               // << "    Relative standard deviation = " << std::setprecision(3) << rsd * 100.0 << " [%]" << G4endl;
    // }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRunAction::OutputVoxelTally(const G4String& prefix,
                                        const G4int numHistory,
                                        G4THitsMap<G4double>* hitsMap,
                                        G4THitsMap<G4double>* hitsMapSquared,
                                        G4bool isDividedByMass)
{
    // convert energy to tally per source particle
    auto detectorConstruction = static_cast<const RoutineDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4double mass = detectorConstruction->GetPhantomMass();
    G4double volume = detectorConstruction->GetPhantomVolume();

    G4double rawTally = 0.0;
    for(auto it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); ++it)
    {
        rawTally += *(it->second);
    }
    G4double tally;

    if(isDividedByMass)
    {
        tally = rawTally / mass;
        tally /= numHistory;
        G4cout << "    Average " << prefix << " = " << tally / (MeV / g) << " [MeV/g]"<< G4endl;
    }
    else
    {
        tally = rawTally / volume;
        tally /= numHistory;
        G4cout << "    Average " << prefix << " = " << tally / (1.0 / (cm * cm)) << " [1/cm2]"<< G4endl;
    }


    // get phantom parameters
    RoutineThreeVector<G4int> numVoxel = detectorConstruction->GetNumVoxel();
    G4double voxelVolume = detectorConstruction->GetVoxelVolume();
    std::ofstream file;
    std::ofstream fileRSD;
    if(rp->param->outputBinaryVoxelTally)
    {
        G4String path = rp->param->outputDir + "/" + prefix + "_tally_" + rp->param->outputSuffix + ".bin";
        G4String pathRSD = rp->param->outputDir + "/" + prefix + "_rsd_" + rp->param->outputSuffix + ".bin";
        file.open(path, std::ofstream::out | std::ofstream::binary);
        fileRSD.open(pathRSD, std::ofstream::out | std::ofstream::binary);

        if(!file)
        {
            G4String msg = "File cannot be opened: " + path;
            G4Exception("RoutineMCNPImporter::InputUniverseList()", "RoutineReport", FatalException, msg);
        }
        else
        {
            G4cout << "    Save voxel tally to " << path << G4endl;
        }

        if(!fileRSD)
        {
            G4String msg = "File cannot be opened: " + pathRSD;
            G4Exception("RoutineMCNPImporter::InputUniverseList()", "RoutineReport", FatalException, msg);
        }
        else
        {
            G4cout << "    Save voxel RSD to " << pathRSD << G4endl;
        }
    }
    else
    {
        G4String path = rp->param->outputDir + "/" + prefix + "_" + rp->param->outputSuffix + ".txt";
        file.open(path, std::ofstream::out);
        if(!file)
        {
            G4String msg = "File cannot be opened: " + path;
            G4Exception("RoutineMCNPImporter::InputUniverseList()", "RoutineReport", FatalException, msg);
        }
        else
        {
            G4cout << "    Save tally and RSD to " << path << G4endl;
        }
    }

    // store data in column major where x index changes fastest
    std::vector<G4double> tally_temp_list(numVoxel.x * numVoxel.y * numVoxel.z);
    std::vector<G4double> rsd_list(numVoxel.x * numVoxel.y * numVoxel.z);

    for(G4int k = 0; k < numVoxel.z; ++k)
    {
        for(G4int j = 0; j < numVoxel.y; ++j)
        {
            for(G4int i = 0; i < numVoxel.x; ++i)
            {
                G4double rawTally_temp = 0.0;
                G4double rawTally2_temp = 0.0;

                // x idx changes fastest, z slowest
                G4int globalIdx = numVoxel.x * numVoxel.y * k + numVoxel.x * j + i;
                G4double* rawTallyPtr = (*hitsMap)[globalIdx];
                if(rawTallyPtr != nullptr)
                {
                    rawTally_temp = *rawTallyPtr;
                }
                G4double* rawTally2Ptr = (*hitsMapSquared)[globalIdx];
                if(rawTally2Ptr != nullptr)
                {
                    rawTally2_temp = *rawTally2Ptr;
                }

                G4double tally_temp;
                G4double tally2_temp;
                if(isDividedByMass)
                {
                    // convert from energy to dose
                    G4Material* material = detectorConstruction->GetPhantomMaterial(globalIdx);
                    G4double density = material->GetDensity();
                    G4double mass_temp = density * voxelVolume;
                    tally_temp  = rawTally_temp / mass_temp;
                    tally2_temp = rawTally2_temp / mass_temp / mass_temp;
                }
                else
                {
                    // convert from track-length to flux
                    tally_temp  = rawTally_temp / voxelVolume;
                    tally2_temp = rawTally2_temp / voxelVolume / voxelVolume;
                }

                G4double sigma = tally2_temp - tally_temp * tally_temp / numHistory;
                if (sigma > 0.0)
                {
                    sigma = std::sqrt(sigma);
                }
                else
                {
                    sigma = 0.0;
                }
                tally_temp /= numHistory;
                sigma /= numHistory;
                G4double rsd;
                if(tally_temp != 0.0)
                {
                    rsd = sigma / tally_temp;
                }
                else
                {
                    rsd = 0.0;
                }

                tally_temp_list[globalIdx] = tally_temp;
                rsd_list[globalIdx] = rsd;

                if(!rp->param->outputBinaryVoxelTally)
                {
                    if(isDividedByMass)
                    {
                        file << std::setw(5) << i
                             << std::setw(5) << j
                             << std::setw(5) << k
                             << std::setw(20) << std::scientific << std::setprecision(12) << tally_temp / (MeV / g)
                             << std::setw(20) << std::scientific << std::setprecision(12) << rsd << G4endl;
                    }
                    else
                    {
                        file << std::setw(5) << i
                             << std::setw(5) << j
                             << std::setw(5) << k
                             << std::setw(20) << std::scientific << std::setprecision(12) << tally_temp / (1.0 / (cm * cm))
                             << std::setw(20) << std::scientific << std::setprecision(12) << rsd << G4endl;
                    }
                }

            }
        }
    }

    if(rp->param->outputBinaryVoxelTally)
    {
        file.write((const char*)tally_temp_list.data(), sizeof(G4double) * tally_temp_list.size());
        fileRSD.write((const char*)rsd_list.data(), sizeof(G4double) * rsd_list.size());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRunAction::AddEdep(G4double edep)
{
    fEdep  += edep;
    fEdep2 += edep * edep;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRunAction::CalculateMeanAndRsd(G4double& mean,
                                           G4double& rsd,
                                           const G4int numHistory,
                                           const G4double x,
                                           const G4double x2)
{
    G4double sigma = x2 - x * x / numHistory;

    if (sigma > 0.0)
    {
        sigma = std::sqrt(sigma);
    }
    else
    {
        sigma = 0.0;
    }

    mean = x / numHistory;
    sigma /= numHistory;

    if(x != 0.0)
    {
        rsd = sigma / mean;
    }
    else
    {
        rsd = 0.0;
    }
}



