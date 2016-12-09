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

class G4VPhysicalVolume;
class G4LogicalVolume;

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineNestedParameterisation : public G4VNestedParameterisation
{
public:
    RoutineNestedParameterisation(const G4ThreeVector& voxelSize,
                                  const G4int xNumVoxel,
                                  const G4int yNumVoxel,
                                  const G4int zNumVoxel,
                                  std::map<G4String, G4Material*>* materialMap,
                                  std::vector<G4Material*>* materialList,
                                  std::vector<G4Material*>* phantomMaterialList);
    ~RoutineNestedParameterisation();

    virtual G4Material* ComputeMaterial(G4VPhysicalVolume* currentPhysicalVol, const G4int zVoxelIdx, const G4VTouchable* parentTouch);
    virtual void ComputeTransformation(const G4int zVoxelIdx, G4VPhysicalVolume* currentPhysicalVol) const;
    virtual void ComputeDimensions(G4Box& box, const G4int zVoxelIdx, const G4VPhysicalVolume* currentPhysicalVol) const;
    virtual G4int GetNumberOfMaterials() const;
    virtual G4Material* GetMaterial(G4int idx) const;
    G4Material* GetPhantomMaterial(G4int globalIdx) const;
private:

    G4double fHalfXDimVoxel;
    G4double fHalfYDimVoxel;
    G4double fHalfZDimVoxel;
    G4int fXNumVoxel;
    G4int fYNumVoxel;
    G4int fZNumVoxel;
    G4int fTotalNumVoxel;

    std::vector<G4double> fZTranslationList;
    std::map<G4String, G4Material*>* fMaterialMap;
    std::vector<G4Material*>* fPhantomMaterialList;
    std::vector<G4Material*>* fMaterialList;
};

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    RoutineDetectorConstruction();
    virtual ~RoutineDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetPhantomDimension(G4ThreeVector dim);
    const G4ThreeVector& GetPhantomDimension() const;
    G4double GetVoxelVolume() const;
    void SetNumVoxel(G4int nx, G4int ny, G4int nz);
    const void GetNumVoxel(G4int& nx, G4int& ny, G4int& nz) const;

    G4LogicalVolume* GetLogicPhantom() const;

    G4LogicalVolume* GetLogicVolumeHighestDepthInPhantom() const;

    RoutineNestedParameterisation* GetParameterisation() const;

    void ImportFromFile();

protected:
    G4LogicalVolume* fLogicVolumeVoxel;
    G4LogicalVolume* fLogicPhantom;
    G4ThreeVector    fPhantomDimension;
    G4ThreeVector    fVoxelDimension;
    G4int            fXNumVoxel;
    G4int            fYNumVoxel;
    G4int            fZNumVoxel;
    G4int            fTotalNumVoxel;

    RoutineNestedParameterisation* fparam;
    std::map<G4String, G4Material*> fMaterialMap;
    std::vector<G4Material*> fPhantomMaterialList;
    std::vector<G4Material*> fMaterialList;
};

#endif

