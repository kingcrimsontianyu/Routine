#include "RoutineDetectorConstruction.hh"


#include "globals.hh"
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
#include "G4PSTrackLength.hh"
#include "RoutineScorer.hh"
#include "RoutineField.hh"
#include "G4VNestedParameterisation.hh"
#include "RoutineParameter.hh"
#include "G4VNestedParameterisation.hh"
#include "RoutineImportPhantom.hh"

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineNestedParameterisation : public G4VNestedParameterisation
{
public:
    RoutineNestedParameterisation(const RoutineThreeVector<G4double>& dimVoxel,
                                  const RoutineThreeVector<G4int>& numVoxel,
                                  std::vector<G4Material*>* phantomList,
                                  std::vector<G4Material*>* materialList);
    ~RoutineNestedParameterisation();

    // possible bug in G4.10.3.2 ???
    // G4VNestedParameterisation::ComputeMaterial() may hide G4VPVParameterisation::ComputeMaterial()
    virtual G4Material* ComputeMaterial(G4VPhysicalVolume* currentPhysicalVol, const G4int xVoxelIdx, const G4VTouchable* parentTouch = 0) override;
    virtual void ComputeTransformation(const G4int xVoxelIdx, G4VPhysicalVolume* currentPhysicalVol) const;
    virtual void ComputeDimensions(G4Box& box, const G4int xVoxelIdx, const G4VPhysicalVolume* currentPhysicalVol) const override;
    virtual G4int GetNumberOfMaterials() const;
    virtual G4Material* GetMaterial(G4int idx) const;
    G4Material* GetPhantomMaterial(G4int globalIdx) const;
private:
    RoutineThreeVector<G4int>      fNumVoxel;
    RoutineThreeVector<G4double>   fHalfDimVoxel;

    std::vector<G4double> fXTranslationList;
    std::vector<G4Material*>* fPhantomList;
    std::vector<G4Material*>* fMaterialList;
};






//------------------------------------------------------------
//------------------------------------------------------------
RoutineDetectorConstruction::RoutineDetectorConstruction(RoutineParameterManager* rp_ext)
: G4VUserDetectorConstruction(),
fLogicVolumeVoxel(nullptr),
fLogicPhantom(nullptr),
rp(rp_ext),
fparam(nullptr),
importer(new RoutineMCNPImporter)
{
    G4cout << "--> ctor: RoutineDetectorConstruction" << G4endl;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineDetectorConstruction::~RoutineDetectorConstruction()
{
    G4cout << "--> dtor: RoutineDetectorConstruction" << G4endl;
    delete importer;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::ImportFromFile()
{
    importer->SetMaterialPath(rp->param->materialPath);
    importer->SetPhantomPath(rp->param->phantomPath);
    importer->SetUniverseToMaterialPath(rp->param->universeToMaterialPath);
    importer->InputPhantom();

    fPhantomList  = importer->PilferPhantomList();
    fMaterialList = importer->GetG4MaterialList();

    fNumVoxel = importer->GetNumVoxel();
    fDimVoxel = importer->GetDimVoxel();
    fDimPhantom.x = fNumVoxel.x * fDimVoxel.x;
    fDimPhantom.y = fNumVoxel.y * fDimVoxel.y;
    fDimPhantom.z = fNumVoxel.z * fDimVoxel.z;

    G4cout << "--> phantom info\n"
           << "    num voxel   = " << std::setw(10) << fNumVoxel.x << std::setw(10) << fNumVoxel.y << std::setw(10) << fNumVoxel.z << G4endl
           << "    dim voxel   = " << std::setw(10) << fDimVoxel.x / cm << std::setw(10) << fDimVoxel.y / cm << std::setw(10) << fDimVoxel.z / cm << G4endl
           << "    dim phantom = " << std::setw(10) << fDimPhantom.x / cm << std::setw(10) << fDimPhantom.y / cm << std::setw(10) << fDimPhantom.z / cm << G4endl;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::AddInternalMaterial()
{
    // internal material
    G4double density;
    std::vector<G4int> ZList;
    std::vector<G4double> weightFractionList;
    G4NistManager* nist = G4NistManager::Instance();

    // dry air
    // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=104
    G4double air_1 = 0.000124;
    G4double air_2 = 0.755267;
    G4double air_3 = 0.231781;
    G4double air_4 = 0.012827;
    G4double air_sum = air_1 + air_2 + air_3 + air_4;
    air_1 /= air_sum;
    air_2 /= air_sum;
    air_3 /= air_sum;
    air_4 /= air_sum;
    ZList.push_back(6 );  weightFractionList.push_back(air_1);
    ZList.push_back(7 );  weightFractionList.push_back(air_2);
    ZList.push_back(8 );  weightFractionList.push_back(air_3);
    ZList.push_back(18);  weightFractionList.push_back(air_4);
    density = 1.20479e-03 * g / cm3;
    G4Material* air = new G4Material("surround_air", density, ZList.size());
    for(size_t i = 0; i < ZList.size(); ++i)
    {
        G4Element* el = nist->FindOrBuildElement(ZList[i]);
        air->AddElement(el, weightFractionList[i]);
    }
    ZList.clear(); weightFractionList.clear();
    fNonPhantomMaterialMap.insert(std::pair<G4String, G4Material*>("surround_air", air));

    // // water
    // // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=276
    // G4double water_1 = 0.111894;
    // G4double water_2 = 0.888106;
    // G4double water_sum = water_1 + water_2;
    // water_1 /= water_sum;
    // water_2 /= water_sum;
    // ZList.push_back(1 );  weightFractionList.push_back(water_1);
    // ZList.push_back(8 );  weightFractionList.push_back(water_2);
    // density = 1.0 * g / cm3;
    // G4Material* water = new G4Material("water", density, ZList.size());
    // for(size_t i = 0; i < ZList.size(); ++i)
    // {
        // G4Element* el = nist->FindOrBuildElement(ZList[i]);
        // water->AddElement(el, weightFractionList[i]);
    // }
    // ZList.clear(); weightFractionList.clear();
    // fNonPhantomMaterialMap.insert(std::pair<G4String, G4Material*>("water", water));
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::PrintMaterialInfo()
{
    G4NistManager* nist = G4NistManager::Instance();
    // get all atomic mass
    for(G4int Z = 1; Z < maxNumElements; ++Z)
    {
        G4cout << "Z = " << std::setw(10) << Z
               << "A = " << std::setprecision(20) << nist->GetAtomicMassAmu(Z) << G4endl;
    }

    // show all material involved in the simulation
    G4MaterialTable* mtable = G4Material::GetMaterialTable();
    G4cout << "--> G4 material aka MCNP universe" << G4endl;
    for(auto&& mat : *mtable)
    {
        G4cout << "    " << std::setw(20) << std::left << mat->GetName()
                         << std::setw(20) << std::setprecision(6) << mat->GetDensity() / (g / cm3) << "\n";
        for(size_t i = 0; i < mat->GetNumberOfElements(); ++i)
        {
            G4Element* el = (*mat->GetElementVector())[i];
            G4double frac = mat->GetFractionVector()[i];
            G4cout << "        " << std::setw(15) << el->GetZ() << std::setw(15) << frac << G4endl;
        }
    }
}

//------------------------------------------------------------
// executed by a single thread
//------------------------------------------------------------
G4VPhysicalVolume* RoutineDetectorConstruction::Construct()
{
    // external phantom materials get registered first
    ImportFromFile();

    // internal material (surround_air gets registered second)
    AddInternalMaterial();
    // PrintMaterialInfo();

    // world
    G4Box* solidWorld = new G4Box("S_World", // name
                                  rp->param->worldDimension.x / 2.0,
                                  rp->param->worldDimension.y / 2.0,
                                  rp->param->worldDimension.z / 2.0);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,                              // solid
                                                      fNonPhantomMaterialMap["surround_air"],  // material
                                                      "L_World");                              // name
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                // no rotation
                                                     G4ThreeVector(),  // at (0,0,0)
                                                     logicWorld,       // logical volume
                                                     "P_World",        // name
                                                     0,                // mother volume
                                                     false,            // no boolean operation
                                                     0,                // copy number
                                                     true);            // overlaps checking

    // phantom
    G4Box* solidphantom = new G4Box("S_Phantom", // name
                                    fDimPhantom.x / 2.0,
                                    fDimPhantom.y / 2.0,
                                    fDimPhantom.z / 2.0);
    fLogicPhantom = new G4LogicalVolume(solidphantom,                           // solid
                                        fNonPhantomMaterialMap["surround_air"], // material
                                        "L_Phantom");                           // name
    // G4VPhysicalVolume* physphantom
    new G4PVPlacement(0,                  // no rotation
                      G4ThreeVector(),    // at (0,0,0)
                      fLogicPhantom,      // logical volume
                      "P_Phantom",        // name
                      logicWorld,         // mother volume
                      false,              // no boolean operation
                      0,                  // copy number
                      true);              // overlaps checking

    // z replica
    G4VSolid* solidZ = new G4Box("S_RepZ", // name
                                 fDimPhantom.x / 2.0,
                                 fDimPhantom.y / 2.0,
                                 fDimVoxel.z   / 2.0);
    G4LogicalVolume* logicZ = new G4LogicalVolume(solidZ,                                  // solid
                                                  fNonPhantomMaterialMap["surround_air"],  // material
                                                  "L_RepZ");                               // name
    // G4PVReplica* physZ
    new G4PVReplica("P_RepZ",
                    logicZ,              // associated logical volume
                    fLogicPhantom,       // associated mother volume, can also be G4VPhysicalVolume*
                    kZAxis,              // axis along witch the replication is applied
                    fNumVoxel.z,         // the number of replicated volumes
                    fDimVoxel.z,         // width of a single replica along the axis of replication
                    0);

    // y replica
    G4VSolid* solidY = new G4Box("S_RepY",  // name
                                 fDimPhantom.x / 2.0,
                                 fDimVoxel.y   / 2.0,
                                 fDimVoxel.z   / 2.0);
    G4LogicalVolume* logicY = new G4LogicalVolume(solidY,                                 // solid
                                                  fNonPhantomMaterialMap["surround_air"], // material
                                                  "L_RepY");                              // name
    // unused G4PVReplica* physY
    new G4PVReplica("P_RepY",
                    logicY,              // associated logical volume
                    logicZ,              // associated mother volume, can also be G4VPhysicalVolume*
                    kYAxis,              // axis along witch the replication is applied
                    fNumVoxel.y,         // the number of replicated volumes
                    fDimVoxel.y,         // width of a single replica along the axis of replication
                    0);

    // x voxel
    G4VSolid* solidX = new G4Box("S_Voxel", // name
                                 fDimVoxel.x / 2.0,
                                 fDimVoxel.y / 2.0,
                                 fDimVoxel.z / 2.0);
    G4LogicalVolume* logicX = new G4LogicalVolume(solidX,                                 // solid
                                                  fNonPhantomMaterialMap["surround_air"], // material
                                                  "L_Voxel");                             // name
    fLogicVolumeVoxel = logicX;

    fparam = new RoutineNestedParameterisation(fDimVoxel,
                                               fNumVoxel,
                                               &fPhantomList,
                                               &fMaterialList);
    // G4VPhysicalVolume* physicalVoxel
    new G4PVParameterised("P_Voxel",         // name
                          fLogicVolumeVoxel, // logical volume
                          logicY,            // mother logical volume
                          kUndefined,        // enable 3D voxelization algorithm
                          fNumVoxel.x,       // number of cells
                          fparam);           // parametrization

    // always return the physical World
    return physWorld;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::ConstructSDandField()
{
    // create mfd and add scorer to it
    G4MultiFunctionalDetector* mfd = new G4MultiFunctionalDetector("PhantomMFD");
    mfd->RegisterPrimitive(new RoutinePSEnergyImparted(G4String("energyImparted3D"), fNumVoxel.x, fNumVoxel.y, fNumVoxel.z));
    mfd->RegisterPrimitive(new RoutinePSEnergyTransfer(G4String("energyTransfer3D"), fNumVoxel.x, fNumVoxel.y, fNumVoxel.z));

    // add mfd to the singleton sd manager
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(mfd);

    // add mfd to logical vol
    fLogicVolumeVoxel->SetSensitiveDetector(mfd);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4double RoutineDetectorConstruction::GetVoxelVolume() const
{
    return fDimVoxel.x * fDimVoxel.y * fDimVoxel.z;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineThreeVector<G4int> RoutineDetectorConstruction::GetNumVoxel() const
{
    return fNumVoxel;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4LogicalVolume* RoutineDetectorConstruction::GetLogicPhantom() const
{
    return fLogicPhantom;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4LogicalVolume* RoutineDetectorConstruction::GetLogicVolumeHighestDepthInPhantom() const
{
    return fLogicVolumeVoxel;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineNestedParameterisation* RoutineDetectorConstruction::GetParameterisation() const
{
    return fparam;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4Material* RoutineDetectorConstruction::GetPhantomMaterial(G4int globalIdx) const
{
    return fparam->GetPhantomMaterial(globalIdx);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4double RoutineDetectorConstruction::GetPhantomMass() const
{
    G4double mass = 0.0;
    G4double voxelVolume = GetVoxelVolume();

    for(G4int k = 0; k < fNumVoxel.z; ++k)
    {
        for(G4int j = 0; j < fNumVoxel.y; ++j)
        {
            for(G4int i = 0; i < fNumVoxel.x; ++i)
            {
                // x idx changes fastest, z slowest
                G4int globalIdx = fNumVoxel.x * fNumVoxel.y * k + fNumVoxel.x * j + i;
                G4Material* material = fparam->GetPhantomMaterial(globalIdx);
                mass += material->GetDensity() * voxelVolume;
            }
        }
    }

    // G4cout << "--> Phantom mass = " << mass / g << " [g]" << G4endl;
    return mass;
}




//------------------------------------------------------------
//------------------------------------------------------------
RoutineNestedParameterisation::RoutineNestedParameterisation(const RoutineThreeVector<G4double>& dimVoxel,
                                                             const RoutineThreeVector<G4int>& numVoxel,
                                                             std::vector<G4Material*>* phantomList,
                                                             std::vector<G4Material*>* materialList) : G4VNestedParameterisation()
{
    fHalfDimVoxel.x = dimVoxel.x / 2.0;
    fHalfDimVoxel.y = dimVoxel.y / 2.0;
    fHalfDimVoxel.z = dimVoxel.z / 2.0;

    fNumVoxel = numVoxel;

    for(int idx = 0; idx < fNumVoxel.x; ++idx)
    {
        // the original voxel template is centered at (0,0,0)
        // we want the whole phantom to be centered at (0,0,0)
        // and we want to determine the shift of each voxel relative to the original template
        // - fHalfDimVoxel.x * fNumVoxel.x + fHalfDimVoxel.x : the center of the 1st voxel
        // increment this center by 2.0 * fHalfDimVoxel.x for each iteration
        G4double trans = - fHalfDimVoxel.x * fNumVoxel.x + fHalfDimVoxel.x + 2.0 * fHalfDimVoxel.x * idx;
        fXTranslationList.push_back(trans);
    }

    fPhantomList  = phantomList;
    fMaterialList = materialList;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineNestedParameterisation::~RoutineNestedParameterisation()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
G4Material* RoutineNestedParameterisation::ComputeMaterial(G4VPhysicalVolume*, const G4int xVoxelIdx, const G4VTouchable* parentTouch)
{
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // protection for initialization and vis at idle state
    // this is necessary
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(!parentTouch)
    {
        return fPhantomList->at(0);
    }

    G4Material* mat = nullptr;
    G4int zVoxelIdx = parentTouch->GetReplicaNumber(1); // parent's (y) parent (z) has depth of 1 relative to parent
    G4int yVoxelIdx = parentTouch->GetReplicaNumber(0); // parent (y) has depth of 0 relative to itself
    G4int globalIdx = fNumVoxel.x * fNumVoxel.y * zVoxelIdx + fNumVoxel.x * yVoxelIdx + xVoxelIdx;
    mat = fPhantomList->at(globalIdx);
    return mat;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineNestedParameterisation::ComputeTransformation(const G4int xVoxelIdx, G4VPhysicalVolume* currentPhysicalVol) const
{
    currentPhysicalVol->SetTranslation(G4ThreeVector(fXTranslationList[xVoxelIdx], 0.0, 0.0));
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineNestedParameterisation::ComputeDimensions(G4Box& box, const G4int, const G4VPhysicalVolume*) const
{
    box.SetXHalfLength(fHalfDimVoxel.x);
    box.SetYHalfLength(fHalfDimVoxel.y);
    box.SetZHalfLength(fHalfDimVoxel.z);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4int RoutineNestedParameterisation::GetNumberOfMaterials() const
{
    return fMaterialList->size();
}

//------------------------------------------------------------
//------------------------------------------------------------
G4Material* RoutineNestedParameterisation::GetMaterial(G4int idx) const
{
    return fMaterialList->at(idx);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4Material* RoutineNestedParameterisation::GetPhantomMaterial(G4int globalIdx) const
{
    return fPhantomList->at(globalIdx);
}




