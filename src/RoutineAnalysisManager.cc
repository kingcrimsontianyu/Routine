#include "RoutineAnalysisManager.hh"
#include "G4PhysicalConstants.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineAnalysisManager::RoutineAnalysisManager()
{
    Initialize();
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineAnalysisManager::~RoutineAnalysisManager()
{
    delete G4AnalysisManager::Instance();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineAnalysisManager::Initialize()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetActivation(true);

    G4int nbins = 1000;
    G4double vmin = std::cos(10.0 / 180.0 * pi);
    // G4double vmax = std::cos(0.0 / 180.0 * pi);
    G4double vmax = 1.0;
    analysisManager->CreateH1("scatter_polar_angle_cosine", "scatter polar angle cosine", nbins, vmin, vmax);
}