#ifndef ROUTINE_HH
#define ROUTINE_HH

#include "RoutineDetectorConstruction.hh"
#include "RoutineActionInitialization.hh"

#ifdef G4MULTITHREADED
	#include "G4MTRunManager.hh"
#else
	#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "RoutinePhysics.hh"


void GetVisInfo(G4VisManager* visManager);

#endif