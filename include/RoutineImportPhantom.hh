#ifndef ROUTINE_IMPORT_PHANTOM_H
#define ROUTINE_IMPORT_PHANTOM_H 1

#include "globals.hh"
#include "G4Material.hh"

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

    void BuildG4MaterialList();

    // std::vector<G4Material*>& GetG4Material() const;

private:
    G4int numVoxelX;
    G4int numVoxelY;
    G4int numVoxelZ;
    G4int totalNumVoxel;
    G4double dimVoxelX;
    G4double dimVoxelY;
    G4double dimVoxelZ;

    G4String materialPath;
    G4String phantomPath;
    G4String universeToMaterialPath;

    std::vector<G4int> universeList; // universe idx
    std::vector<UniverseToMCNPMaterialBlob> universeToMCNPMaterialList; // universe idx, density, material idx
    std::vector<MCNPMaterialBlob> mcnpMaterialList; // universe name, density, elemental composition
    std::vector<G4Material*> g4MaterialList;

    void InputUniverseList();
    void InputMCNPMaterial();
    void InputUniverseToMaterial();
    void AnalyzeUniverseList();
};

#endif


