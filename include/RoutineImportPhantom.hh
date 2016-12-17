#ifndef ROUTINE_IMPORT_PHANTOM_H
#define ROUTINE_IMPORT_PHANTOM_H 1

#include "globals.hh"
#include "G4Material.hh"

class UniverseToMaterialData
{
    G4String universeName;
    G4int    universeIdx;
    G4double density;
    G4int    materialIdx;
};

class RoutineImportPhantom
{
public:
    RoutineImportPhantom();
    virtual ~RoutineImportPhantom();

    std::vector<G4Material*>& GetG4Material() const;

private:
    G4String materialPath;
    G4String phantomPath;
    G4String universeToMaterialPath;

    std::vector<int> phantomList; // universe idx
    std::vector<UniverseToMaterialData> universeToMaterial; // universe idx, density, material idx
    std::vector<G4Material*> universeList; // universe name, density, elemental composition
};

#endif


