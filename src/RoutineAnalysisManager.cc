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
    G4double vmax = 1.00000001;
    analysisManager->CreateH1("single_scatter_polar_angle_cosine", "single scatter polar angle cosine", nbins, vmin, vmax);
    analysisManager->CreateH1("multiple_scatter_polar_angle_cosine", "multiple scatter polar angle cosine", nbins, vmin, vmax);
}