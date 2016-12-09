#ifndef ROUTINE_RUN_H
#define ROUTINE_RUN_H 1

#include "G4Run.hh"
#include "G4Event.hh"

#include "G4THitsMap.hh"
#include <vector>

class RoutineRun : public G4Run
{

public:
    RoutineRun(G4String);
    virtual ~RoutineRun();

public:
    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);

    G4int GetNumberOfHitsMap() const {return fRunMap.size();}
    G4THitsMap<G4double>* GetHitsMap(const G4String& fullName) const;
    G4THitsMap<G4double>* GetHitsMapSquared(const G4String& fullName) const;
    void GetHitsMapAndSquaredSum(G4THitsMap<G4double>*& hitsMap,
                                 G4double& squaredSum,
                                 const G4String& fullName) const;
    void DumpAllScorer();

private:
    std::vector<G4String> fCollectionName;
    std::vector<G4int> fCollectionID;
    std::vector<G4THitsMap<G4double>*> fRunMap;
    std::vector<G4double> fRunMapSquaredSum;
    std::vector<G4THitsMap<G4double>*> fRunMapSquared;
};

#endif
