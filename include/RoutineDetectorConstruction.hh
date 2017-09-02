#ifndef ROUTINE_DETECTORCONSTRUCTION_H
#define ROUTINE_DETECTORCONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VNestedParameterisation.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVParameterised.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "RoutineScorer.hh"
#include "RoutineField.hh"
#include "G4AutoDelete.hh"
#include "RoutineParameter.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class RoutineNestedParameterisation;

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
};

#endif

