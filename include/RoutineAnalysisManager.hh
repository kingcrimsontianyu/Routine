#ifndef ROUTINE_ANALYSIS_MANAGER_H
#define ROUTINE_ANALYSIS_MANAGER_H 1

#include "globals.hh"
#include "g4csv.hh"

class RoutineAnalysisManager
{
public:
    RoutineAnalysisManager();
    virtual ~RoutineAnalysisManager();

protected:
    void Initialize();
};

#endif


