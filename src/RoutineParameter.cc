#include "RoutineParameter.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineParameterValue::RoutineParameterValue(const G4String& rawString, ParamType valueType) :
rawString(rawString), valueType(valueType), single(""), x(""), y(""), z(""), flag(false)
{
    // todo: add validity check

    if(valueType == ParamType::Single)
    {
        single = rawString;
    }
    else if(valueType == ParamType::ThreeVector)
    {
        // first split by "," delimiter
        G4String delimiter = ",";
        auto oldPos = rawString.find(delimiter); // first ","
        auto pos = rawString.find(delimiter, oldPos + 1); // second ","

        x = rawString.substr(0, oldPos);
        y = rawString.substr(oldPos + 1, pos - oldPos - 1);
        z = rawString.substr(pos + 1);
    }
    else if(valueType == ParamType::Boolean)
    {
        if(rawString == "true")
        {
            flag = true;
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineParameterManager::RoutineParameterManager(int argc,char** argv)
{
    InitializeMap();
    ParseCommandLine(argc, argv);
    SetParameter();
    PrintParameterMap();
}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineParameterManager::RoutineParameterManager()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
RoutineParameterManager::~RoutineParameterManager()
{
    delete param; param = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineParameterManager::InitializeMap()
{
    // predefined parameter map
    parameterMap["source-type"        ] = RoutineParameterValue("proton", ParamType::Single); // proton, gamma, e-, e+
    parameterMap["source-energy"      ] = RoutineParameterValue("200", ParamType::Single); // MeV
    parameterMap["source-position"    ] = RoutineParameterValue("0, 0, -10.1", ParamType::ThreeVector); // cm
    parameterMap["source-direction"   ] = RoutineParameterValue("0, 0, 1", ParamType::ThreeVector);
    parameterMap["square-beam-width"  ] = RoutineParameterValue("1", ParamType::Single); // cm
    parameterMap["world-dim"          ] = RoutineParameterValue("80, 80, 80", ParamType::ThreeVector); // cm
    parameterMap["phantom-dim"        ] = RoutineParameterValue("20, 20, 20", ParamType::ThreeVector); // cm
    parameterMap["num-voxel"          ] = RoutineParameterValue("1, 1, 100", ParamType::ThreeVector);
    parameterMap["num-thread"         ] = RoutineParameterValue("10", ParamType::Single);
    parameterMap["num-history"        ] = RoutineParameterValue("1e3", ParamType::Single);
    parameterMap["magnetic-field"     ] = RoutineParameterValue("0, 0, 0", ParamType::ThreeVector); // Tesla
    parameterMap["source-ion-Z"       ] = RoutineParameterValue("", ParamType::Single);
    parameterMap["source-ion-A"       ] = RoutineParameterValue("", ParamType::Single);
    parameterMap["output-suffix"      ] = RoutineParameterValue("", ParamType::Single);
    parameterMap["store-physics-table"] = RoutineParameterValue("false", ParamType::Boolean);
    parameterMap["physics-table-dir"  ] = RoutineParameterValue("xs_data", ParamType::Single);
}

//------------------------------------------------------------
// at least in bash, unquoted space characters in argv are
// automatically removed !!!
//------------------------------------------------------------
void RoutineParameterManager::ParseCommandLine(int argc, char** argv)
{
    if(argc > 1)
    {
        // concatenate all parameters
        G4String rawParam;
        for (int i = 1; i < argc; ++i)
        {
            rawParam += argv[i];
        }

        // split the string by "--" delimiter
        std::vector<G4String> parsedParam;
        G4String delimiter = "--";
        size_t found = rawParam.find(delimiter);
        if (found != std::string::npos)
        {
            rawParam.erase(found, delimiter.length());
            do
            {
                size_t found = rawParam.find(delimiter);
                G4String extract;

                if (found != std::string::npos)
                {
                    extract = rawParam.substr(0, found);
                    parsedParam.push_back(extract);
                }
                else
                {
                    extract = rawParam;
                    parsedParam.push_back(extract);
                    break;
                }

                rawParam.erase(0, extract.length() + delimiter.length());
            }
            while(true);
        }

        // split each element by "=" delimiter if it exists
        for(G4int i = 0; i < parsedParam.size(); ++i)
        {
            G4String delimiter = "=";
            size_t found = parsedParam[i].find(delimiter);
            G4String cmdKey, cmdValue;
            if (found != std::string::npos)
            {
                cmdKey = parsedParam[i].substr(0, found);
                cmdValue = parsedParam[i].substr(found + 1);
            }
            else // if "=" does not exist, the parameter is Boolean type
            {
                cmdKey = parsedParam[i].substr(0, found);
                cmdValue = "true";
            }

            // check if cmdKey exists in the preset map
            auto it = parameterMap.find(cmdKey);
            if(it != parameterMap.end())
            {
                // assign the cmd (non-default), validated parameter to the map
                // ctor of RoutineParameterValue() shall guarantee validity
                auto old = parameterMap[it->first];
                parameterMap[it->first] = RoutineParameterValue(cmdValue, old.valueType);
            }
            else
            {
                throw std::invalid_argument("Routine: parameter does not exist --> " + cmdKey);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineParameterManager::SetParameter()
{
    param = new RoutineParameter;

    // if both source-ion-Z and source-ion-A is specified by the user
    if(!parameterMap["source-ion-Z"].single.empty() &&
       !parameterMap["source-ion-A"].single.empty())
    {
        param->sourceType = "none";
        param->isIonSource = true;
        param->sourceIonZ = std::stoi(parameterMap["source-ion-Z"].single);
        param->sourceIonA = std::stoi(parameterMap["source-ion-A"].single);
    }
    else
    {
        param->sourceType = parameterMap["source-type"].single;
        param->isIonSource = false;
        param->sourceIonZ = 0;
        param->sourceIonA = 0;
    }


    param->sourceEnergy = std::stof(parameterMap["source-energy"].single) * MeV;
    param->squareBeamWidth = std::stof(parameterMap["square-beam-width"].single) * cm;

    param->sourcePosition.x = std::stof(parameterMap["source-position"].x) * cm;
    param->sourcePosition.y = std::stof(parameterMap["source-position"].y) * cm;
    param->sourcePosition.z = std::stof(parameterMap["source-position"].z) * cm;

    param->sourceDirection.x = std::stof(parameterMap["source-direction"].x);
    param->sourceDirection.y = std::stof(parameterMap["source-direction"].y);
    param->sourceDirection.z = std::stof(parameterMap["source-direction"].z);

    param->worldDimension.x = std::stof(parameterMap["world-dim"].x) * cm;
    param->worldDimension.y = std::stof(parameterMap["world-dim"].y) * cm;
    param->worldDimension.z = std::stof(parameterMap["world-dim"].z) * cm;

    param->phantomDimension.x = std::stof(parameterMap["phantom-dim"].x) * cm;
    param->phantomDimension.y = std::stof(parameterMap["phantom-dim"].y) * cm;
    param->phantomDimension.z = std::stof(parameterMap["phantom-dim"].z) * cm;

    param->numVoxel.x = std::stoi(parameterMap["num-voxel"].x);
    param->numVoxel.y = std::stoi(parameterMap["num-voxel"].y);
    param->numVoxel.z = std::stoi(parameterMap["num-voxel"].z);

    param->numThread = std::stoi(parameterMap["num-thread"].single);
    param->numHistory = static_cast<G4int>(std::stof(parameterMap["num-history"].single));

    param->magneticField.x = std::stof(parameterMap["magnetic-field"].x) * tesla;
    param->magneticField.y = std::stof(parameterMap["magnetic-field"].y) * tesla;
    param->magneticField.z = std::stof(parameterMap["magnetic-field"].z) * tesla;

    param->outputSuffix = parameterMap["output-suffix"].single;

    param->storePhysicsTable = parameterMap["store-physics-table"].flag;
    param->physicsTableDir = parameterMap["physics-table-dir"].single;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineParameterManager::PrintParameterMap()
{
    G4cout << "--> Parameter list" << G4endl;
    for(auto it = parameterMap.begin(); it != parameterMap.end(); ++it)
    {
        if(it->second.valueType == ParamType::Single)
        {
            G4cout << "    "
                   << std::left << std::setw(20) << it->first
                   << std::setw(20) << it->second.single << G4endl;
        }
        else if(it->second.valueType == ParamType::ThreeVector)
        {
            G4cout << "    "
                   << std::left << std::setw(20) << it->first
                   << std::setw(20) << it->second.x
                   << std::setw(20) << it->second.y
                   << std::setw(20) << it->second.z
                   << G4endl;
        }
        else if(it->second.valueType == ParamType::Boolean)
        {
            G4cout << "    "
                               << std::left << std::setw(20) << it->first
                               << std::setw(20) << it->second.flag << G4endl;
        }
    }
    G4cout << G4endl;
}

