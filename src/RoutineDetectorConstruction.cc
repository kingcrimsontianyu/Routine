#include "RoutineDetectorConstruction.hh"

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

    // possible bug in G4.10.3.2 ???
    // G4VNestedParameterisation::ComputeMaterial() may hide G4VPVParameterisation::ComputeMaterial()
    virtual G4Material* ComputeMaterial(G4VPhysicalVolume* currentPhysicalVol, const G4int xVoxelIdx, const G4VTouchable* parentTouch = 0) override;

    virtual void ComputeTransformation(const G4int xVoxelIdx, G4VPhysicalVolume* currentPhysicalVol) const;
    virtual void ComputeDimensions(G4Box& box, const G4int xVoxelIdx, const G4VPhysicalVolume* currentPhysicalVol) const override;
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

    std::vector<G4double> fXTranslationList;
    std::map<G4String, G4Material*>* fMaterialMap;
    std::vector<G4Material*>* fPhantomMaterialList;
    std::vector<G4Material*>* fMaterialList;
};

//------------------------------------------------------------
//------------------------------------------------------------
RoutineDetectorConstruction::RoutineDetectorConstruction(RoutineParameterManager* rp_ext)
: G4VUserDetectorConstruction(),
fLogicVolumeVoxel(nullptr),
fLogicPhantom(nullptr),
rp(rp_ext),
fparam(nullptr)
{ }

//------------------------------------------------------------
//------------------------------------------------------------
RoutineDetectorConstruction::~RoutineDetectorConstruction()
{ }

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::ImportFromFile()
{
    G4double density;
    // G4double weightFraction;
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
    G4Material* air = new G4Material("air", density, ZList.size());
    for(size_t i = 0; i < ZList.size(); ++i)
    {
        G4Element* el = nist->FindOrBuildElement(ZList[i]);
        air->AddElement(el, weightFractionList[i]);
    }
    ZList.clear(); weightFractionList.clear();
    fMaterialMap.insert(std::pair<G4String, G4Material*>("air", air));

    // water
    // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=276
    G4double water_1 = 0.111894;
    G4double water_2 = 0.888106;
    G4double water_sum = water_1 + water_2;
    water_1 /= water_sum;
    water_2 /= water_sum;
    ZList.push_back(1 );  weightFractionList.push_back(water_1);
    ZList.push_back(8 );  weightFractionList.push_back(water_2);
    density = 1.0 * g / cm3;
    G4Material* water = new G4Material("water", density, ZList.size());
    for(size_t i = 0; i < ZList.size(); ++i)
    {
        G4Element* el = nist->FindOrBuildElement(ZList[i]);
        water->AddElement(el, weightFractionList[i]);
    }
    ZList.clear(); weightFractionList.clear();
    fMaterialMap.insert(std::pair<G4String, G4Material*>("water", water));

    // get all atomic mass
    for(G4int Z = 1; Z < maxNumElements; ++Z)
    {
        G4cout << "Z = " << std::setw(10) << Z
               << "A = " << std::setprecision(20) << nist->GetAtomicMassAmu(Z) << G4endl;
    }

    // // tissue, soft (icrp)
    // // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=261
    // ZList.push_back(1 );  weightFractionList.push_back(0.104472);
    // ZList.push_back(6 );  weightFractionList.push_back(0.232190);
    // ZList.push_back(7 );  weightFractionList.push_back(0.024880);
    // ZList.push_back(8 );  weightFractionList.push_back(0.630238);
    // ZList.push_back(11);  weightFractionList.push_back(0.001130);
    // ZList.push_back(12);  weightFractionList.push_back(0.000130);
    // ZList.push_back(15);  weightFractionList.push_back(0.001330);
    // ZList.push_back(16);  weightFractionList.push_back(0.001990);
    // ZList.push_back(17);  weightFractionList.push_back(0.001340);
    // ZList.push_back(19);  weightFractionList.push_back(0.001990);
    // ZList.push_back(20);  weightFractionList.push_back(0.000230);
    // ZList.push_back(26);  weightFractionList.push_back(0.000050);
    // ZList.push_back(30);  weightFractionList.push_back(0.000030);
    // density = 1.0 * g / cm3;
    // G4Material* softTissue = new G4Material("soft_tissue", density, ZList.size());
    // for(int i = 0; i < ZList.size(); ++i)
    // {
        // G4Element* el = nist->FindOrBuildElement(ZList[i]);
        // softTissue->AddElement(el, weightFractionList[i]);
    // }
    // ZList.clear(); weightFractionList.clear();
    // fMaterialMap.insert(std::pair<G4String, G4Material*>("soft_tissue", softTissue));


    // // bone, cortical (icrp)
    // // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=120
    // ZList.push_back(1 );  weightFractionList.push_back(0.047234);
    // ZList.push_back(6 );  weightFractionList.push_back(0.144330);
    // ZList.push_back(7 );  weightFractionList.push_back(0.041990);
    // ZList.push_back(8 );  weightFractionList.push_back(0.446096);
    // ZList.push_back(12);  weightFractionList.push_back(0.002200);
    // ZList.push_back(15);  weightFractionList.push_back(0.104970);
    // ZList.push_back(16);  weightFractionList.push_back(0.003150);
    // ZList.push_back(20);  weightFractionList.push_back(0.209930);
    // ZList.push_back(30);  weightFractionList.push_back(0.000100);
    // density = 1.85 * g / cm3;
    // G4Material* bone = new G4Material("bone", density, ZList.size());
    // for(int i = 0; i < ZList.size(); ++i)
    // {
        // G4Element* el = nist->FindOrBuildElement(ZList[i]);
        // bone->AddElement(el, weightFractionList[i]);
    // }
    // ZList.clear(); weightFractionList.clear();
    // fMaterialMap.insert(std::pair<G4String, G4Material*>("bone", bone));

    // // lead
    // // http://physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=082
    // ZList.push_back(82);  weightFractionList.push_back(1);
    // density = 11.35 * g / cm3;
    // G4Material* lead = new G4Material("lead", density, ZList.size());
    // for(int i = 0; i < ZList.size(); ++i)
    // {
        // G4Element* el = nist->FindOrBuildElement(ZList[i]);
        // lead->AddElement(el, weightFractionList[i]);
    // }
    // ZList.clear(); weightFractionList.clear();
    // fMaterialMap.insert(std::pair<G4String, G4Material*>("lead", lead));

    // // vacuum
    // G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    // fMaterialMap.insert(std::pair<G4String, G4Material*>("vacuum", vacuum));


    fPhantomDimension.set(rp->param->phantomDimension.x,
                          rp->param->phantomDimension.y,
                          rp->param->phantomDimension.z);

    fXNumVoxel = rp->param->numVoxel.x;
    fYNumVoxel = rp->param->numVoxel.y;
    fZNumVoxel = rp->param->numVoxel.z;
    fTotalNumVoxel = fXNumVoxel * fYNumVoxel * fZNumVoxel;

    fVoxelDimension.set(fPhantomDimension.x() / fXNumVoxel,
                        fPhantomDimension.y() / fYNumVoxel,
                        fPhantomDimension.z() / fZNumVoxel);

    fPhantomMaterialList.resize(fTotalNumVoxel);
    for(G4int k = 0; k < fZNumVoxel; ++k)
    {
        for(G4int j = 0; j < fYNumVoxel; ++j)
        {
            for(G4int i = 0; i < fXNumVoxel; ++i)
            {
                G4int globalIdx = fXNumVoxel * fYNumVoxel * k + fXNumVoxel * j + i;

                // if(k == 0)
                // {
                    // fPhantomMaterialList[globalIdx] = fMaterialMap["soft_tissue"];
                // }
                // else
                // {
                    // fPhantomMaterialList[globalIdx] = fMaterialMap["bone"];
                // }

                fPhantomMaterialList[globalIdx] = fMaterialMap["water"];
            }
        }
    }

    for(auto it = fMaterialMap.begin(); it != fMaterialMap.end(); ++it)
    {
        fMaterialList.push_back(it->second);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
G4VPhysicalVolume* RoutineDetectorConstruction::Construct()
{
    ImportFromFile();

    // World
    G4Box* solidWorld = new G4Box("S_World",        // name
                                  rp->param->worldDimension.x / 2.0,
                                  rp->param->worldDimension.y / 2.0,
                                  rp->param->worldDimension.z / 2.0);     // size

    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  // solid
                                                      fMaterialMap["air"],   // material
                                                      "L_World");    // name

    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                // no rotation
                                                     G4ThreeVector(),  // at (0,0,0)
                                                     logicWorld,       // logical volume
                                                     "P_World",          // name
                                                     0,                // mother volume
                                                     false,            // no boolean operation
                                                     0,                // copy number
                                                     true);            // overlaps checking

    // Phantom
    G4Box* solidphantom = new G4Box("S_Phantom",        // name
                                    fPhantomDimension.x() / 2.0,
                                    fPhantomDimension.y() / 2.0,
                                    fPhantomDimension.z() / 2.0);     // size

    fLogicPhantom = new G4LogicalVolume(solidphantom,  // solid
                                        fMaterialMap["air"],   // material
                                        "L_Phantom");    // name

    // unused G4VPhysicalVolume* physphantom
    new G4PVPlacement(0,                  // no rotation
                      G4ThreeVector(0.0, 0.0, 0.0),    // at (0,0,0)
                      fLogicPhantom,      // logical volume
                      "P_Phantom",          // name
                      logicWorld,         // mother volume
                      false,              // no boolean operation
                      0,                  // copy number
                      true);              // overlaps checking

    // z replica
    G4VSolid* solidZ = new G4Box("S_RepZ",        // name
                                  fPhantomDimension.x() / 2.0,
                                  fPhantomDimension.y() / 2.0,
                                  fVoxelDimension.z() / 2.0);     // size

    G4LogicalVolume* logicZ = new G4LogicalVolume(solidZ,  // solid
                                                  fMaterialMap["air"],   // material
                                                  "L_RepZ");    // name

    // unused G4PVReplica* physZ
    new G4PVReplica("P_RepZ",
                    logicZ,
                    fLogicPhantom, // OR G4VPhysicalVolume*
                    kZAxis,
                    fZNumVoxel,
                    fVoxelDimension.z(),
                    0);

    // y replica
    G4VSolid* solidY = new G4Box("S_RepY",        // name
                                 fPhantomDimension.x() / 2.0,
                                 fVoxelDimension.y() / 2.0,
                                 fVoxelDimension.z() / 2.0);     // size

    G4LogicalVolume* logicY = new G4LogicalVolume(solidY,  // solid
                                                  fMaterialMap["air"],   // material
                                                  "L_RepY");    // name

    // unused G4PVReplica* physY
    new G4PVReplica("P_RepY",
                    logicY,
                    logicZ, // OR G4VPhysicalVolume*
                    kYAxis,
                    fYNumVoxel,
                    fVoxelDimension.y(),
                    0);

    // x voxel
    G4VSolid* solidX = new G4Box("S_Voxel",        // name
                                 fVoxelDimension.x() / 2.0,
                                 fVoxelDimension.y() / 2.0,
                                 fVoxelDimension.z() / 2.0);     // size

    G4LogicalVolume* logicX = new G4LogicalVolume(solidX,  // solid
                                                  fMaterialMap["air"],   // material
                                                  "L_Voxel");    // name

    fLogicVolumeVoxel = logicX;

    fparam = new RoutineNestedParameterisation(fVoxelDimension,
                                               fXNumVoxel,
                                               fYNumVoxel,
                                               fZNumVoxel,
                                               &fMaterialMap,
                                               &fMaterialList,
                                               &fPhantomMaterialList);

    // unused G4VPhysicalVolume* physicalVoxel
    new G4PVParameterised("P_Voxel",         // name
                          fLogicVolumeVoxel, // logical volume
                          logicY,            // mother logical volume
                          kUndefined,
                          fXNumVoxel,        // number of cells
                          fparam);            // parameterisation.

    // print info
    // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // always return the physical World
    return physWorld;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::ConstructSDandField()
{
    // create mfd and add scorer to it
    G4MultiFunctionalDetector* mfd = new G4MultiFunctionalDetector("PhantomMFD");
    mfd->RegisterPrimitive(new G4PSEnergyDeposit("totalD"));
    mfd->RegisterPrimitive(new RoutinePSEnergyImparted(G4String("energyImparted3D"), fXNumVoxel, fYNumVoxel, fZNumVoxel));
    mfd->RegisterPrimitive(new RoutinePSEnergyTransfer(G4String("energyTransfer3D"), fXNumVoxel, fYNumVoxel, fZNumVoxel));

    // add mfd to the singleton sd manager
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(mfd);

    // add mfd to logical vol
    fLogicVolumeVoxel->SetSensitiveDetector(mfd);

    // construct the field creator
    if (!fField.Get())
    {
        RoutineField* field = new RoutineField(G4ThreeVector(rp->param->magneticField.x, rp->param->magneticField.y, rp->param->magneticField.z));
        G4AutoDelete::Register(field); // Kernel will delete the RoutineField
        fField.Put(field);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::SetPhantomDimension(G4ThreeVector dim)
{
    fPhantomDimension = dim;
}

//------------------------------------------------------------
//------------------------------------------------------------
const G4ThreeVector& RoutineDetectorConstruction::GetPhantomDimension() const
{
    return fPhantomDimension;
}

//------------------------------------------------------------
//------------------------------------------------------------
G4double RoutineDetectorConstruction::GetVoxelVolume() const
{
    return fVoxelDimension.x() * fVoxelDimension.y() * fVoxelDimension.z();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::SetNumVoxel(G4int nx, G4int ny, G4int nz)
{
    fXNumVoxel = nx;
    fYNumVoxel = ny;
    fZNumVoxel = nz;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineDetectorConstruction::GetNumVoxel(G4int& nx, G4int& ny, G4int& nz) const
{
    nx = fXNumVoxel;
    ny = fYNumVoxel;
    nz = fZNumVoxel;
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
    G4double voxelVolume = fVoxelDimension.x() * fVoxelDimension.y() * fVoxelDimension.z();

    for(G4int k = 0; k < fZNumVoxel; ++k)
    {
        for(G4int j = 0; j < fYNumVoxel; ++j)
        {
            for(G4int i = 0; i < fXNumVoxel; ++i)
            {
                // x idx changes fastest, z slowest
                G4int globalIdx = fXNumVoxel * fYNumVoxel * k + fXNumVoxel * j + i;
                G4Material* material = fparam->GetPhantomMaterial(globalIdx);
                mass += material->GetDensity() * voxelVolume;
            }
        }
    }

    G4cout << "--> phantom mass = " << mass / g << " [g]" << G4endl;
    return mass;
}




//------------------------------------------------------------
//------------------------------------------------------------
RoutineNestedParameterisation::RoutineNestedParameterisation(const G4ThreeVector& voxelSize,
                                                             const G4int xNumVoxel,
                                                             const G4int yNumVoxel,
                                                             const G4int zNumVoxel,
                                                             std::map<G4String, G4Material*>* materialMap,
                                                             std::vector<G4Material*>* materialList,
                                                             std::vector<G4Material*>* phantomMaterialList) : G4VNestedParameterisation()
{
    fHalfXDimVoxel = voxelSize.x() / 2.0;
    fHalfYDimVoxel = voxelSize.y() / 2.0;
    fHalfZDimVoxel = voxelSize.z() / 2.0;

    fXNumVoxel = xNumVoxel;
    fYNumVoxel = yNumVoxel;
    fZNumVoxel = zNumVoxel;
    fTotalNumVoxel = xNumVoxel * yNumVoxel * zNumVoxel;

    for(int idx = 0; idx < fXNumVoxel; ++idx)
    {
        // the original voxel template is centered at (0,0,0)
        // we want the whole phantom to be centered at (0,0,0)
        // and we want to determine the shift of each voxel relative to the original template
        // - fHalfZDimVoxel * fZNumVoxel + fHalfZDimVoxel : the center of the 1st voxel
        // increment this center by 2.0 * fHalfZDimVoxel for each iteration
        G4double trans = - fHalfXDimVoxel * fXNumVoxel + fHalfXDimVoxel + 2.0 * fHalfXDimVoxel * idx;
        fXTranslationList.push_back(trans);
    }

    fMaterialMap = materialMap;
    fMaterialList = materialList;
    fPhantomMaterialList = phantomMaterialList;
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineNestedParameterisation::~RoutineNestedParameterisation()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
G4Material* RoutineNestedParameterisation::ComputeMaterial(G4VPhysicalVolume*, const G4int, const G4VTouchable* parentTouch)
{
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // protection for initialization and vis at idle state
    // this is necessary
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(!parentTouch)
    {
        return fMaterialList->at(0);
    }

    G4Material* mat = nullptr;
    // G4int zVoxelIdx = parentTouch->GetReplicaNumber(1); // parent's (y) parent (z) has depth of 1 relative to parent
    // G4int yVoxelIdx = parentTouch->GetReplicaNumber(0); // parent (y) has depth of 0 relative to itself
    // G4int globalIdx = fXNumVoxel * fYNumVoxel * zVoxelIdx + fXNumVoxel * yVoxelIdx + xVoxelIdx;

    // if(zVoxelIdx == 0)
    // {
        // mat = fMaterialMap->at("soft_tissue");
    // }
    // else
    // {
        // mat = fMaterialMap->at("bone");
    // }
    mat = fMaterialMap->at("water");

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
    box.SetXHalfLength(fHalfXDimVoxel);
    box.SetYHalfLength(fHalfYDimVoxel);
    box.SetZHalfLength(fHalfZDimVoxel);
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
    return fPhantomMaterialList->at(globalIdx);
}




