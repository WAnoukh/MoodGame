#include "WorldReader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

WorldReader::WorldReader(World& worldToLoad): world(&worldToLoad)
{
    
}

int WorldReader::Load()
{
    if (path == nullptr)
    {
        std::cerr << "No path set for loading" << std::endl;
        return -1;
    }
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Could not open file " << path << std::endl;
        return -1;
    }
    int returnValue = 0;
    std::string line;
    enum {NONE, SECTORS, SECTORSNUM, WALLS, WALLSNUM} section = NONE;
    size_t numSectors = 0;
    size_t numWalls = 0;
    size_t currentSector = 0;
    size_t currentWall = 0;
    Sector* sectors = nullptr;
    Wall* walls = nullptr;
    while(std::getline(file, line))
    {
        if(returnValue != 0)
        {
            break;
        }
        if (line.empty() or line[0] == '#')
        {
            continue;
        }
        switch (section)
        {
            case NONE:
                if (!line.compare("[SECTOR]"))
                {
                    section = SECTORSNUM;
                }
                else if (!line.compare("[WALL]"))
                {
                    section = WALLSNUM;
                }
                break;
            case SECTORSNUM:
                numSectors = std::stoi(line);
                sectors = new Sector[numSectors];
                section = SECTORS;
                break;
            case WALLSNUM:
                numWalls = std::stoi(line);
                walls = new Wall[numWalls];
                section = WALLS;
                break;
            case SECTORS:
                {
                    std::istringstream iss(line);
                    if(sectors == nullptr)
                    {
                        std::cerr << "Reading sector before having their numbers" << std::endl;
                        returnValue = -3;
                        break;
                    }
                    Sector& sector = sectors[currentSector];
                    int sectorNum;
                    if(!(iss >> sectorNum >> sector.firstWall >> sector.numWalls >> sector.floor >> sector.ceil))
                    {
                        std::cerr << "Can't read sector from line" << std::endl;
                        returnValue = -2;
                        break;
                    }
                    currentSector++;
                    if(currentSector == numSectors)
                    {
                        section = NONE;
                    }
                }
            break;
            case WALLS:
                {
                    std::istringstream iss(line);
                    if(walls == nullptr)
                    {
                        std::cerr << "Reading wall before having their numbers" << std::endl;
                        returnValue = -5;
                        break;
                    }
                    Wall& wall = walls[currentWall];
                    if(!(iss >> wall.x1 >> wall.y1 >> wall.x2 >> wall.y2 >> wall.portal))
                    {
                        std::cerr << "Can't read wall from line" << std::endl;
                        returnValue = -4;
                        break;
                    }
                    currentWall++;
                    if(currentWall == numWalls)
                    {
                        section = NONE;
                    }
                }
            break;
        }
    }
    if (currentSector != numSectors)
    {
        std::cerr << "Read " << currentSector << " sectors but expected " << numSectors << std::endl;
        returnValue = -6;
    }
    if (currentWall != numWalls)
    {
        std::cerr << "Read " << currentWall << " walls but expected " << numWalls << std::endl;
        returnValue = -7;
    }
    if(numSectors <= 0)
    {
        std::cerr << "No sectors to load" << std::endl;
        returnValue = -8;
    }
    if(numWalls <= 0)
    {
        std::cerr << "No walls to load" << std::endl;
        returnValue = -9;
    }
    if(returnValue == 0)
    {
        world->SetSectors(sectors, numSectors);
        world->SetWalls(walls, numWalls);
    }
    else
    {
        delete[] sectors;
        delete[] walls;
    }
    file.close();
    return returnValue;
}

int WorldReader::Save(const char* filePath)
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        std::cerr << "Could not open file " << filePath << std::endl;
        return -1;
    }
    outFile << "[SECTOR]\n";
    outFile << world->GetSectorsCount() << "\n";
    for (size_t i = 1; i <= world->GetSectorsCount(); ++i)
    {
        Sector* sector = world->GetSector(i);
        outFile << i << " " << sector->firstWall << " " << sector->numWalls << " " << sector->floor << " " << sector->ceil << "\n";
    }
    outFile << "[WALL]\n";
    outFile << world->GetWallsCount() << "\n";
    for (size_t i = 0; i < world->GetWallsCount(); ++i)
    {
        Wall* wall = world->GetWall(i);
        outFile << wall->x2 << " " << wall->y2 << " " << wall->x1 << " " << wall->y1 << " " << wall->portal << "\n";
    }
    outFile.close();
    return 0;
}

void WorldReader::SetPath(const char* filePath)
{
    path = filePath;
}
