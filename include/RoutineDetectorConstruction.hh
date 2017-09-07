#ifndef ROUTINE_DETECTORCONSTRUCTION_H
#define ROUTINE_DETECTORCONSTRUCTION_H 1

#include "G4AutoDelete.hh"
#include "G4ThreeVector.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Material;
class RoutineField;
class RoutineParameterManager;
class G4VPhysicalVolume;
class G4LogicalVolume;
class RoutineNestedParameterisation;
class RoutineMCNPImporter;

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    RoutineDetectorConstruction(RoutineParameterManager* rp);
    virtual ~RoutineDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetPhantomDimension(G4ThreeVector dim);
    const G4ThreeVector& GetPhantomDimension() const;
    G4double GetVoxelVolume() const;
    void SetNumVoxel(G4int nx, G4int ny, G4int nz);
    void GetNumVoxel(G4int& nx, G4int& ny, G4int& nz) const;

    G4LogicalVolume* GetLogicPhantom() const;

    G4LogicalVolume* GetLogicVolumeHighestDepthInPhantom() const;

    RoutineNestedParameterisation* GetParameterisation() const;

    void ImportFromFile();
    G4Material* GetPhantomMaterial(G4int globalIdx) const;
    G4double GetPhantomMass() const;
protected:
    G4LogicalVolume* fLogicVolumeVoxel;
    G4LogicalVolume* fLogicPhantom;
    G4ThreeVector    fPhantomDimension;
    G4ThreeVector    fVoxelDimension;
    G4int            fXNumVoxel;
    G4int            fYNumVoxel;
    G4int            fZNumVoxel;
    G4int            fTotalNumVoxel;

    RoutineParameterManager* rp;
    RoutineNestedParameterisation* fparam;
    std::map<G4String, G4Material*> fMaterialMap;
    std::vector<G4Material*> fPhantomMaterialList;
    std::vector<G4Material*> fMaterialList;

    G4Cache<RoutineField*> fField;

    RoutineMCNPImporter* importer;
};

#endif

