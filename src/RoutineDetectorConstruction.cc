#include "RoutineDetectorConstruction.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineDetectorConstruction::RoutineDetectorConstruction() : G4VUserDetectorConstruction(),
                                                           fLogicPhantom(nullptr),
                                                           fLogicVolumeVoxel(nullptr),
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
    G4double weightFraction;
    std::vector<G4int> ZList;
    std::vector<G4double> weightFractionList;

    G4NistManager* nist = G4NistManager::Instance();

    // dry air
    // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=104
    ZList.push_back(6 );  weightFractionList.push_back(0.000124);
    ZList.push_back(7 );  weightFractionList.push_back(0.755267);
    ZList.push_back(8 );  weightFractionList.push_back(0.231781);
    ZList.push_back(18);  weightFractionList.push_back(0.012827);
    density = 1.20479e-03 * g / cm3;
    G4Material* air = new G4Material("air", density, ZList.size());
    for(int i = 0; i < ZList.size(); ++i)
    {
        G4Element* el = nist->FindOrBuildElement(ZList[i]);
        air->AddElement(el, weightFractionList[i]);
    }
    ZList.clear(); weightFractionList.clear();
    fMaterialMap.insert(std::pair<G4String, G4Material*>("air", air));

    // tissue, soft (icrp)
    // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=261
    ZList.push_back(1 );  weightFractionList.push_back(0.104472);
    ZList.push_back(6 );  weightFractionList.push_back(0.232190);
    ZList.push_back(7 );  weightFractionList.push_back(0.024880);
    ZList.push_back(8 );  weightFractionList.push_back(0.630238);
    ZList.push_back(11);  weightFractionList.push_back(0.001130);
    ZList.push_back(12);  weightFractionList.push_back(0.000130);
    ZList.push_back(15);  weightFractionList.push_back(0.001330);
    ZList.push_back(16);  weightFractionList.push_back(0.001990);
    ZList.push_back(17);  weightFractionList.push_back(0.001340);
    ZList.push_back(19);  weightFractionList.push_back(0.001990);
    ZList.push_back(20);  weightFractionList.push_back(0.000230);
    ZList.push_back(26);  weightFractionList.push_back(0.000050);
    ZList.push_back(30);  weightFractionList.push_back(0.000030);
    density = 1.0 * g / cm3;
    G4Material* softTissue = new G4Material("soft_tissue", density, ZList.size());
    for(int i = 0; i < ZList.size(); ++i)
    {
        G4Element* el = nist->FindOrBuildElement(ZList[i]);
        softTissue->AddElement(el, weightFractionList[i]);
    }
    ZList.clear(); weightFractionList.clear();
    fMaterialMap.insert(std::pair<G4String, G4Material*>("soft_tissue", softTissue));


    // bone, cortical (icrp)
    // http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=120
    ZList.push_back(1 );  weightFractionList.push_back(0.047234);
    ZList.push_back(6 );  weightFractionList.push_back(0.144330);
    ZList.push_back(7 );  weightFractionList.push_back(0.041990);
    ZList.push_back(8 );  weightFractionList.push_back(0.446096);
    ZList.push_back(12);  weightFractionList.push_back(0.002200);
    ZList.push_back(15);  weightFractionList.push_back(0.104970);
    ZList.push_back(16);  weightFractionList.push_back(0.003150);
    ZList.push_back(20);  weightFractionList.push_back(0.209930);
    ZList.push_back(30);  weightFractionList.push_back(0.000100);
    density = 1.85 * g / cm3;
    G4Material* bone = new G4Material("bone", density, ZList.size());
    for(int i = 0; i < ZList.size(); ++i)
    {
        G4Element* el = nist->FindOrBuildElement(ZList[i]);
        bone->AddElement(el, weightFractionList[i]);
    }
    ZList.clear(); weightFractionList.clear();
    fMaterialMap.insert(std::pair<G4String, G4Material*>("bone", bone));

    fPhantomDimension.set(10.0 * cm, 10.0 * cm, 10.0 * cm);

    fXNumVoxel = 2;
    fYNumVoxel = 2;
    fZNumVoxel = 2;
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

                if(k == 0)
                {
                    fPhantomMaterialList[globalIdx] = fMaterialMap["soft_tissue"];
                }
                else
                {
                    fPhantomMaterialList[globalIdx] = fMaterialMap["bone"];
                }
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
                                  100.0 * cm,
                                  100.0 * cm,
                                  100.0 * cm);     // size

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

    G4VPhysicalVolume* physphantom = new G4PVPlacement(0,                  // no rotation
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

    G4PVReplica* physZ = new G4PVReplica("P_RepZ",
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

    G4PVReplica* physY = new G4PVReplica("P_RepY",
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
    G4VPhysicalVolume* physicalVoxel = new G4PVParameterised("P_Voxel",         // name
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
    mfd->RegisterPrimitive(new RoutinePSEnergyDeposit(G4String("totalD3D"), fXNumVoxel, fYNumVoxel, fZNumVoxel));

    // add mfd to the singleton sd manager
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(mfd);

    // add mfd to logical vol
    fLogicVolumeVoxel->SetSensitiveDetector(mfd);
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
const void RoutineDetectorConstruction::GetNumVoxel(G4int& nx, G4int& ny, G4int& nz) const
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

    for(size_t idx = 0; idx < fZNumVoxel; ++idx)
    {
        // the original voxel template is centered at (0,0,0)
        // we want the whole phantom to be centered at (0,0,0)
        // and we want to determine the shift of each voxel relative to the original template
        // - fHalfZDimVoxel * fZNumVoxel + fHalfZDimVoxel : the center of the 1st voxel
        // increment this center by 2.0 * fHalfZDimVoxel for each iteration
        G4double trans = - fHalfZDimVoxel * fZNumVoxel + fHalfZDimVoxel + 2.0 * fHalfZDimVoxel * idx;
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
G4Material* RoutineNestedParameterisation::ComputeMaterial(G4VPhysicalVolume* currentPhysicalVol, const G4int xVoxelIdx, const G4VTouchable* parentTouch)
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
    G4int zVoxelIdx = parentTouch->GetReplicaNumber(1); // parent's (y) parent (z) has depth of 1 relative to parent
    G4int yVoxelIdx = parentTouch->GetReplicaNumber(0); // parent (y) has depth of 0 relative to itself
    G4int globalIdx = fXNumVoxel * fYNumVoxel * zVoxelIdx + fXNumVoxel * yVoxelIdx + xVoxelIdx;

    if(zVoxelIdx == 0)
    {
        mat = fMaterialMap->at("soft_tissue");
    }
    else
    {
        mat = fMaterialMap->at("bone");
    }

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
void RoutineNestedParameterisation::ComputeDimensions(G4Box& box, const G4int xVoxelIdx, const G4VPhysicalVolume* currentPhysicalVol) const
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




