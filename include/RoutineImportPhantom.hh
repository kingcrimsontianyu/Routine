#ifndef ROUTINE_IMPORT_PHANTOM_H
#define ROUTINE_IMPORT_PHANTOM_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "RoutineParameter.hh"

// universe in mcnp is similar to material in G4, both having a density and elemental composition
// material in mcnp does not have a density

struct UniverseToMCNPMaterialBlob
{
    // G4String universeName;
    G4int    universeIdx;
    G4double density;
    G4int    mcnpMaterialIdx;
};

struct MCNPMaterialBlob
{
    G4int    mcnpMaterialIdx;
    std::vector<G4int> ZList;
    std::vector<G4double> weightFractionList;
};

class RoutineMCNPImporter
{
public:
    RoutineMCNPImporter();
    virtual ~RoutineMCNPImporter();

    void SetMaterialPath(const G4String& path);
    void SetPhantomPath(const G4String& path);
    void SetUniverseToMaterialPath(const G4String& path);

    void InputPhantom();

    void BuildG4MaterialListAndPhantomList();

    // do not access the pilfered data member after these functions are called
    std::vector<G4Material*>&& PilferPhantomList();

    const std::vector<G4Material*>& GetG4MaterialList() const;

    RoutineThreeVector<G4int>    GetNumVoxel();
    RoutineThreeVector<G4double> GetDimVoxel();

private:
    RoutineThreeVector<G4int>      numVoxel;
    RoutineThreeVector<G4double>   dimVoxel;
    G4int totalNumVoxel;
    G4String materialPath;
    G4String phantomPath;
    G4String universeToMaterialPath;

    std::vector<G4int>                      universeList; // universe idx
    std::vector<UniverseToMCNPMaterialBlob> universeToMCNPMaterialList; // universe idx, density, material idx
    std::vector<MCNPMaterialBlob>           mcnpMaterialList; // universe name, density, elemental composition
    std::vector<G4Material*>                g4MaterialList; // each G4 material is equivalent to an mcnp universe
    std::vector<G4Material*>                phantomList;

    void InputUniverseList();
    void InputMCNPMaterial();
    void InputUniverseToMaterial();
    void AnalyzeUniverseList();
};

#endif


