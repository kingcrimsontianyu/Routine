#include "RoutineImportPhantom.hh"
#include "RoutineParameter.hh"

//------------------------------------------------------------
//------------------------------------------------------------
RoutineMCNPImporter::RoutineMCNPImporter()
{}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::SetMaterialPath(const G4String& path)
{
    materialPath = path;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::SetPhantomPath(const G4String& path)
{
    phantomPath = path;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::SetUniverseToMaterialPath(const G4String& path)
{
    universeToMaterialPath = path;
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::InputPhantom()
{
    InputUniverseList();
    // AnalyzeUniverseList();

    InputMCNPMaterial();
    InputUniverseToMaterial();
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::InputUniverseList()
{
    G4cout << "--> Input phantom" << G4endl;

    std::ifstream myFile(phantomPath, std::ifstream::in);
    if(!myFile)
    {
        G4String msg = "File does not exist: " + phantomPath;
        G4Exception("RoutineMCNPImporter::InputUniverseList()", "RoutineReport", FatalException, msg);
    }
    else
    {
        G4String line;
        G4int lineIdx = 0;
        G4int counter = 0;
        G4int repetition;
        std::map<G4String, G4String> tempMap;
        G4int universe = -1;
        while(std::getline(myFile, line))
        {
            ++lineIdx;

            // read the first six lines
            if(lineIdx <= 6)
            {
                // remove space chars
                line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
                auto pos = line.find('=');
                auto key = line.substr(0, pos);
                auto value = line.substr(pos + 1);
                tempMap[key] = value;
            }

            if(lineIdx == 6)
            {
                numVoxelX = std::stoi(tempMap["num-voxel-x"]);
                numVoxelY = std::stoi(tempMap["num-voxel-y"]);
                numVoxelZ = std::stoi(tempMap["num-voxel-z"]);
                dimVoxelX = std::stod(tempMap["dim-voxel-x"]);
                dimVoxelY = std::stod(tempMap["dim-voxel-y"]);
                dimVoxelZ = std::stod(tempMap["dim-voxel-z"]);
                totalNumVoxel = numVoxelX * numVoxelY * numVoxelZ;
                universeList.resize(totalNumVoxel);
            }

            if(lineIdx > 6)
            {
                std::istringstream is(line);
                G4String temp;
                while(is >> temp)
                {
                    // if the string ends with an r
                    if(temp.back() == 'r')
                    {
                        // remove the r
                        repetition = std::stoi(temp.substr(0, temp.length() - 1));

                        // copy the universe by repetition times
                        for (int i = 0; i < repetition; ++i)
                        {
                            universeList[counter + i] = universe;
                        }

                        // increment the counter
                        counter += repetition;
                    }
                    else
                    {
                        // convert from string to int
                        universe = std::stoi(temp);
                        universeList[counter] = universe;
                        ++counter;
                    }
                }
            }
        }

        G4cout << "    Total number of voxels (claimed) = " << totalNumVoxel << G4endl;
        G4cout << "    Total number of voxels (actual)  = " << counter << G4endl;

        if(totalNumVoxel != counter)
        {
            G4String msg = "Phantom dimension mismatches";
            G4Exception("RoutineMCNPImporter::InputUniverseList()", "RoutineReport", FatalException, msg);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::AnalyzeUniverseList()
{
    G4cout << "--> Analyze phantom" << G4endl;

    // sort phantom into ascending order
    std::vector<G4int> uniqueUniverseList;
    uniqueUniverseList.assign(totalNumVoxel, 0);
    std::copy(universeList.begin(), universeList.end(), uniqueUniverseList.begin());
    std::sort(uniqueUniverseList.begin(), uniqueUniverseList.end());

    // remove duplicated elements
    std::vector<int>::iterator it = std::unique(uniqueUniverseList.begin(), uniqueUniverseList.end());
    uniqueUniverseList.resize(std::distance(uniqueUniverseList.begin(), it));

    // count frequency
    std::vector<G4int> uniqueUniverseCountList;
    uniqueUniverseCountList.assign(uniqueUniverseList.size(), 0);
    for (size_t i = 0; i < universeList.size(); ++i)
    {
        int idx = (std::lower_bound(uniqueUniverseList.begin(), uniqueUniverseList.end(), universeList[i]) - uniqueUniverseList.begin());
        ++uniqueUniverseCountList[idx];
    }

    G4cout << "    " << std::left << std::setw(20) << "index"
                     << std::left << std::setw(20) << "count" << G4endl;
    for(size_t i = 0; i < uniqueUniverseList.size(); ++i)
    {
        G4cout << "    " << std::left << std::setw(20) << uniqueUniverseList[i]
                         << std::left << std::setw(20) << uniqueUniverseCountList[i] << G4endl;
    }

}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::InputMCNPMaterial()
{
    G4cout << "--> Input MCNP material" << G4endl;

    std::ifstream myFile(materialPath, std::ifstream::in);
    if(!myFile)
    {
        G4String msg = "File does not exist: " + materialPath;
        G4Exception("RoutineMCNPImporter::InputMCNPMaterial()", "RoutineReport", FatalException, msg);
    }
    else
    {
        G4String line;
        while(std::getline(myFile, line))
        {
            // skip the comment line or empty line
            if(line.length() == 0 || // empty line
              (line.length() == 1 && (line[0] == 'C' || line[0] == 'c')) || //comment line with a single C or c character
              (line.length() > 1 && (line[0] == 'C' || line[0] == 'c') && (line[1] == ' '))) //comment line starting with a single C or c character plus a space
            {
                continue;
            }
            else
            {
                std::istringstream is(line);
                G4String temp;
                is >> temp;

                // if the line starts with m
                if(temp[0] == 'm')
                {
                    // remove ``m'' char
                    temp = temp.substr(1);

                    // extract material idx
                    MCNPMaterialBlob mBlob;
                    mBlob.mcnpMaterialIdx = std::stoi(temp);
                    mcnpMaterialList.push_back(mBlob);
                }
                else
                {
                    // get Z
                    // temp is AAAZZZ
                    // remove AAA and only retain ZZZ
                    temp.erase(temp.length() - 3, 3);

                    mcnpMaterialList.back().ZList.push_back(std::stoi(temp));

                    // get weight fraction
                    is >> temp;
                    mcnpMaterialList.back().weightFractionList.push_back(std::stod(temp) * -1.0);
                }
            }
        }

        // normalize weight fraction
        // G4 does not do it for us!!!
        for(auto&& item : mcnpMaterialList)
        {
            G4double sum = 0.0;
            for(size_t i = 0; i < item.ZList.size(); ++i)
            {
                sum += item.weightFractionList[i];
            }

            for(size_t i = 0; i < item.ZList.size(); ++i)
            {
                item.weightFractionList[i] /= sum;
            }
        }

        // display
        // for(auto&& item : mcnpMaterialList)
        // {
            // G4cout << "    m" << item.mcnpMaterialIdx << G4endl;
            // for(size_t i = 0; i < item.ZList.size(); ++i)
            // {
                // G4cout << "        " << std::setw(15) << item.ZList[i]
                                     // << std::setw(15) << std::setprecision(8) << item.weightFractionList[i] << G4endl;
            // }
        // }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void RoutineMCNPImporter::InputUniverseToMaterial()
{
    G4cout << "--> Input universe-to-material" << G4endl;

    std::ifstream myFile(universeToMaterialPath, std::ifstream::in);
    if(!myFile)
    {
        G4String msg = "File does not exist: " + universeToMaterialPath;
        G4Exception("RoutineMCNPImporter::InputUniverseToMaterial()", "RoutineReport", FatalException, msg);
    }
    else
    {
        G4String line;
        while(std::getline(myFile, line))
        {
            // skip the comment line or empty line
            if(line.length() == 0 || // empty line
              (line.length() == 1 && (line[0] == 'C' || line[0] == 'c')) || //comment line with a single C or c character
              (line.length() > 1 && (line[0] == 'C' || line[0] == 'c') && (line[1] == ' '))) //comment line starting with a single C or c character plus a space
            {
                continue;
            }
            else
            {
                std::istringstream is(line);

                UniverseToMCNPMaterialBlob uBlob;
                is >> uBlob.universeIdx >> uBlob.mcnpMaterialIdx >> uBlob.density;
                uBlob.density *= -1.0;
                universeToMCNPMaterialList.push_back(uBlob);
            }
        }

        // // display
        // for(auto&& item : universeToMCNPMaterialList)
        // {
            // G4cout << "    " << std::setw(10) << item.universeIdx
                             // << std::setw(10) << item.mcnpMaterialIdx
                             // << std::setw(10) << item.density
                             // << G4endl;
        // }
    }
}




