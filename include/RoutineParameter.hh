#ifndef ROUTINE_PARAMETER_H
#define ROUTINE_PARAMETER_H 1

#include "G4Types.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4ios.hh"
#include <map>
#include <vector>
#include <iomanip>
#include <stdexcept>

//------------------------------------------------------------
//------------------------------------------------------------
template<class T>
class RoutineThreeVector
{
public:
    T x;
    T y;
    T z;
};

//------------------------------------------------------------
//------------------------------------------------------------
enum class ParamType
{
    Single,   // single non-boolean value
    ThreeVector,
    Boolean,     // single boolean value
};

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineParameterValue
{
public:
    RoutineParameterValue(const G4String& rawString, ParamType valueType);
    RoutineParameterValue() {};
    ~RoutineParameterValue() {};

    G4String rawString;
    ParamType valueType;

    G4String single;
    G4String x;
    G4String y;
    G4String z;
    G4bool flag;
};

//------------------------------------------------------------
// directly exposed to users
//------------------------------------------------------------
class RoutineParameter
{
public:
    G4String                     sourceType;
    G4double                     sourceEnergy;
    G4double                     squareBeamWidth;
    G4bool                       isIonSource;
    G4int                        sourceIonZ;
    G4int                        sourceIonA;
    RoutineThreeVector<G4double> sourcePosition;
    RoutineThreeVector<G4double> sourceDirection;
    RoutineThreeVector<G4double> worldDimension;
    RoutineThreeVector<G4double> phantomDimension;
    RoutineThreeVector<G4int>    numVoxel;
    G4int                        numThread;
    G4int                        numHistory;
    RoutineThreeVector<G4double> magneticField;
    G4String                     outputSuffix;
    G4bool                       storePhysicsTable;
    G4String                     physicsTableDir;
    G4String                     phantomPath;
    G4String                     materialPath;
    G4String                     universeToMaterialPath;
    G4bool                       logPhysicsInfo;
    G4bool                       outputBinaryVoxelTally;
    G4String                     outputDir;
};

//------------------------------------------------------------
//------------------------------------------------------------
class RoutineParameterManager
{
public:
    RoutineParameterManager(int argc,char** argv);
    ~RoutineParameterManager();
    void InitializeMap();
    void ParseCommandLine(int argc,char** argv);
    void SetParameter();
    void PrintParameterMap();
    RoutineParameter* param; // directly exposed to users
protected:
    RoutineParameterManager();
    std::map<G4String, RoutineParameterValue> parameterMap;
};



#endif