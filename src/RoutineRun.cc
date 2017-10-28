#include "RoutineRun.hh"
#include "G4SDManager.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

//------------------------------------------------------------
// allocate run scope scoring container
//------------------------------------------------------------
RoutineRun::RoutineRun(G4String mfdName) :
G4Run(),
fTrueRange(0.0),
fTrueRange2(0.0),
fProjectedRange(0.0),
fProjectedRange2(0.0)
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    auto mfd = static_cast<G4MultiFunctionalDetector*>(sdManager->FindSensitiveDetector(mfdName));

    if(mfd)
    {
        // loop over primitive scorer
        for (G4int idx = 0; idx < mfd->GetNumberOfPrimitives(); ++idx)
        {
            G4VPrimitiveScorer* scorer = mfd->GetPrimitive(idx);
            G4String collectionName = scorer->GetName();
            G4String fullCollectionName = mfdName + "/" + collectionName;
            G4int collectionID = sdManager->GetCollectionID(fullCollectionName);

            fCollectionName.push_back(fullCollectionName);
            fCollectionID.push_back(collectionID);
            fRunMap.push_back(new G4THitsMap<G4double>(mfdName, collectionName));
            fRunMapSquared.push_back(new G4THitsMap<G4double>(mfdName, collectionName + "Squared"));
            fRunMapSquaredSum.push_back(0.0);
        }

    }
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineRun::~RoutineRun()
{
}

//------------------------------------------------------------
// accumulate event scope scoring container to run scope counterpart
//------------------------------------------------------------
void RoutineRun::RecordEvent(const G4Event* aEvent)
{
    G4Run::RecordEvent(aEvent); // ++numberOfEvent;

    G4HCofThisEvent* hc = aEvent->GetHCofThisEvent();

    if(hc)
    {
        for(size_t i = 0; i < fCollectionID.size(); ++i)
        {
            auto hitsMap = static_cast<G4THitsMap<G4double>*>(hc->GetHC(fCollectionID[i]));

            if(hitsMap)
            {
                G4double temp = 0.0;
                for(auto it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); ++it)
                {
                    // energy per voxel
                    fRunMap[i]->add(it->first, *(it->second));

                    // energy squared per voxel
                    G4double value = (*(it->second));
                    value = value * value;
                    fRunMapSquared[i]->add(it->first, value);

                    temp += (*(it->second));
                }
                // energy over all voxels
                fRunMapSquaredSum[i] += temp * temp;
            }
        }
    }
}


//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRun::Merge(const G4Run * aRun)
{
    const RoutineRun* localRun = static_cast<const RoutineRun*>(aRun);
    for(size_t i = 0; i < fCollectionID.size(); ++i)
    {
        *fRunMap[i] += *localRun->fRunMap[i];
        *fRunMapSquared[i] += *localRun->fRunMapSquared[i];
        fRunMapSquaredSum[i] += localRun->fRunMapSquaredSum[i];
    }

    // merge custom score
    for(auto it = localRun->csMap.begin(); it != localRun->csMap.end(); ++it)
    {
        if(csMap.find(it->first) != csMap.end())
        {
            csMap[it->first] += it->second;
        }
        else
        {
            csMap[it->first] = it->second;
        }
    }

    // other
    fTrueRange       += localRun->fTrueRange      ;
    fTrueRange2      += localRun->fTrueRange2     ;
    fProjectedRange  += localRun->fProjectedRange ;
    fProjectedRange2 += localRun->fProjectedRange2;

    G4Run::Merge(aRun);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4THitsMap<G4double>* RoutineRun::GetHitsMap(const G4String& fullName) const
{
    G4THitsMap<G4double>* result = nullptr;
    for(size_t i = 0; i < fCollectionID.size(); ++i)
    {
        if(fCollectionName[i] == fullName)
        {
            result = fRunMap[i];
        }
    }
    return result;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4THitsMap<G4double>* RoutineRun::GetHitsMapSquared(const G4String& fullName) const
{
    G4THitsMap<G4double>* result = nullptr;
    for(size_t i = 0; i < fCollectionID.size(); ++i)
    {
        if(fCollectionName[i] + "Squared" == fullName)
        {
            result = fRunMapSquared[i];
        }
    }
    return result;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRun::AddTrueRange(G4double x)
{
    fTrueRange  += x;
    fTrueRange2 += x * x;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineRun::AddProjectedRange(G4double x)
{
    fProjectedRange  += x;
    fProjectedRange2 += x * x;
}






