#ifndef ROUTINE_DETECTORCONSTRUCTION_H
#define ROUTINE_DETECTORCONSTRUCTION_H 1

#include "G4AutoDelete.hh"
#include "G4ThreeVector.hh"
#include "G4VUserDetectorConstruction.hh"
#include "RoutineParameter.hh"

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

    G4double GetVoxelVolume() const;
    RoutineThreeVector<G4int> GetNumVoxel() const;

    G4LogicalVolume* GetLogicPhantom() const;

    G4LogicalVolume* GetLogicVolumeHighestDepthInPhantom() const;

    RoutineNestedParameterisation* GetParameterisation() const;
    G4Material* GetPhantomMaterial(G4int globalIdx) const;
    G4double GetPhantomMass() const;
    G4double GetPhantomVolume() const;
protected:
    void AddInternalMaterial();
    void ImportFromFile();
    void PrintMaterialInfo();

    G4LogicalVolume* fLogicVolumeVoxel;
    G4LogicalVolume* fLogicPhantom;

    std::map<G4String, G4Material*> fNonPhantomMaterialMap;
    RoutineParameterManager* rp;
    RoutineNestedParameterisation* fparam;
    std::vector<G4Material*> fMaterialList; // dim == num of mcnp universes in the phantom
    std::vector<G4Material*> fPhantomList; // dim == num of voxels in the phantom
    G4Cache<RoutineField*> fField;
    RoutineMCNPImporter* importer;
    RoutineThreeVector<G4int>      fNumVoxel;
    RoutineThreeVector<G4double>   fDimVoxel;
    RoutineThreeVector<G4double>   fDimPhantom;
};

#endif

