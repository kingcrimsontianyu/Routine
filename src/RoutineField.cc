#include "RoutineField.hh"


//------------------------------------------------------------
//------------------------------------------------------------
RoutineField::RoutineField(G4ThreeVector fieldVector)
            : fFieldManager(0),
              fChordFinder(0),
              fEquation(0),
              fMagneticField(new G4UniformMagField(fieldVector)),
              fStepper(0),
              fStepperType(0),
              fMinStep(0.0)
{
    G4cout << " RoutineField: magnetic field set to Uniform( " << fieldVector << " ) " << G4endl;
    InitialiseAll();
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineField::RoutineField()
            : fFieldManager(0),
              fChordFinder(0),
              fEquation(0),
              fMagneticField(new G4UniformMagField(G4ThreeVector())),
              fStepper(0),
              fStepperType(0),
              fMinStep(0.0)
{
    G4cout << " RoutineField: magnetic field set to Uniform( 0, 0, 0 ) " << G4endl;
    InitialiseAll();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineField::InitialiseAll()
{
    fEquation = new G4Mag_UsualEqRhs(fMagneticField);
    fMinStep = 1.0 * mm; // minimal step of 1 mm is default
    fStepperType = 13;
    fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    CreateStepperAndChordFinder();
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineField::~RoutineField()
{
    delete fMagneticField;
    delete fChordFinder;
    delete fStepper;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineField::CreateStepperAndChordFinder()
{
    // Update field
    G4cout<< "    RoutineField::CreateStepperAndChordFinder() called " << " to reset Stepper."  << G4endl;

    SetStepper();

    G4cout<<"    The minimal step is equal to "<< fMinStep/mm << " mm" << G4endl;

    fFieldManager->SetDetectorField(fMagneticField);

    if (fChordFinder)
    {
        delete fChordFinder;
    }

    fChordFinder = new G4ChordFinder(fMagneticField, fMinStep, fStepper);

    fFieldManager->SetChordFinder( fChordFinder );
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineField::SetStepper()
{
    // Set stepper according to the stepper type
    if (fStepper)
    {
        delete fStepper;
    }

    switch ( fStepperType )
    {
    case 0:
        fStepper = new G4ExplicitEuler( fEquation );
        G4cout<<"G4ExplicitEuler is chosen."<<G4endl;
        break;
    case 1:
        fStepper = new G4ImplicitEuler( fEquation );
        G4cout<<"G4ImplicitEuler is chosen"<<G4endl;
        break;
    case 2:
        fStepper = new G4SimpleRunge( fEquation );
        G4cout<<"G4SimpleRunge is chosen"<<G4endl;
        break;
    case 3:
        fStepper = new G4SimpleHeum( fEquation );
        G4cout<<"G4SimpleHeum is chosen"<<G4endl;
        break;
    case 4:
        fStepper = new G4ClassicalRK4( fEquation );
        G4cout<<"G4ClassicalRK4 (default) is chosen"<<G4endl;
        break;
    case 5:
        fStepper = new G4HelixExplicitEuler( fEquation );
        G4cout<<"G4HelixExplicitEuler is chosen"<<G4endl;
        break;
    case 6:
        fStepper = new G4HelixImplicitEuler( fEquation );
        G4cout<<"G4HelixImplicitEuler is chosen"<<G4endl;
        break;
    case 7:
        fStepper = new G4HelixSimpleRunge( fEquation );
        G4cout<<"G4HelixSimpleRunge is chosen"<<G4endl;
        break;
    case 8:
        fStepper = new G4CashKarpRKF45( fEquation );
        G4cout<<"G4CashKarpRKF45 is chosen"<<G4endl;
        break;
    case 9:
        fStepper = new G4RKG3_Stepper( fEquation );
        G4cout<<"G4RKG3_Stepper is chosen"<<G4endl;
        break;
    case 10:
        fStepper = new G4ExactHelixStepper( fEquation );
        G4cout<<"G4ExactHelixStepper is chosen"<<G4endl;
        break;
    case 11:
        fStepper = new G4HelixMixedStepper( fEquation );
        G4cout<<"G4HelixMixedStepper is chosen"<<G4endl;
        break;
    case 12:
        fStepper = new G4ConstRK4( fEquation );
        G4cout<<"G4ConstRK4 Stepper is chosen"<<G4endl;
        break;
    case 13:
        fStepper = new G4NystromRK4( fEquation );
        G4cout<<" G4NystromRK4 Stepper is chosen"<<G4endl;
        break;
    default: // fStepper = 4;
        fStepper = new G4ClassicalRK4( fEquation );
        G4cout<<"G4ClassicalRK4 Stepper (default) is chosen"<<G4endl;
        break;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineField::SetFieldValue(G4ThreeVector fieldVector)
{
    if (fMagneticField)
    {
        delete fMagneticField;
    }

    if (fieldVector != G4ThreeVector(0.0, 0.0, 0.0))
    {
        fMagneticField = new G4UniformMagField(fieldVector);
    }
    else
    {
        // If the new field's value is Zero, signal it as below
        // so that it is not used for propagation.
        fMagneticField = 0;
    }

    // Set this as the field of the global Field Manager
    GetGlobalFieldManager()->SetDetectorField(fMagneticField);

    // Now notify equation of new field
    fEquation->SetFieldObj(fMagneticField);
}

//------------------------------------------------------------
//------------------------------------------------------------
G4FieldManager* RoutineField::GetGlobalFieldManager()
{
    return G4TransportationManager::GetTransportationManager()->GetFieldManager();
}
