#ifndef ROUTINE_FIELD_H
#define ROUTINE_FIELD_H 1

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4ConstRK4.hh"
#include "G4NystromRK4.hh"
#include "G4HelixMixedStepper.hh"
#include "G4ExactHelixStepper.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;

class RoutineField
{
public:
    RoutineField(G4ThreeVector);  //  The value of the field
    RoutineField();               //  A zero field

    virtual ~RoutineField();

    void SetStepperType( G4int i )
    {
        fStepperType = i;
        CreateStepperAndChordFinder();
    }

    void SetStepper();

    void SetMinStep(G4double s_ext) { fMinStep = s_ext; }

    void InitialiseAll();    //  Set parameters and call method below
    void CreateStepperAndChordFinder();

    void SetFieldValue(G4ThreeVector fieldVector);
    void SetFieldValue(G4double      fieldValue);
    G4ThreeVector GetConstantFieldValue();

protected:

    // Find the global Field Manager
    G4FieldManager*         GetGlobalFieldManager();
    G4FieldManager*         fFieldManager;
    G4ChordFinder*          fChordFinder;
    G4Mag_UsualEqRhs*       fEquation;
    G4MagneticField*        fMagneticField;
    G4MagIntegratorStepper* fStepper;
    G4int                   fStepperType;
    G4double                fMinStep;
};

#endif