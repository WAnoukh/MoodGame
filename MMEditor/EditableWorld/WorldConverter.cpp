#include "WorldConverter.h"

#include <corecrt_math_defines.h>
#include <iostream>

#include "EditableWorld.h"
#include "../../src/World/World.h"
#include <map>
#include <list>
#include <tuple>
using namespace std;

void WorldConverter::ConvertWorld(EditableWorld& editableWorld, World& world)
{
    editableWorld.corners.clear();
    editableWorld.rooms.clear();
    map<tuple<float, float>, int> cornersMap;
    size_t sectorIndex;
    for (sectorIndex = 1; sectorIndex <= world.GetSectorsCount(); ++sectorIndex)
    {
        Sector& sector = *world.GetSector(sectorIndex);
        editableWorld.rooms.emplace_back();
        EditableWorld::Room& room = editableWorld.rooms.back();
        room.ceil = sector.ceil;
        room.floor = sector.floor;
        for (size_t wallIndex = sector.firstWall; wallIndex < sector.firstWall + sector.numWalls; ++wallIndex)
        {
            Wall& wall = *world.GetWall(wallIndex);
            tuple c1Key(wall.x1, wall.y1);
            if(!cornersMap.contains(c1Key))
            {
                editableWorld.corners.push_back({wall.x1,wall.y1});
                EditableWorld::Corner& corner = editableWorld.corners.back();
                cornersMap[c1Key] = editableWorld.corners.size() - 1;
            }
            room.cornersIndexes.push_back(cornersMap[c1Key]);
        }
    }
}

void WorldConverter::ConvertEditableWorld(World& outWorld, const EditableWorld& editableWorld)
{
    std::vector<Wall> walls;
    std::map<tuple<float, float, float, float>, tuple<int, size_t>> wallSectors;
    std::vector<Sector> sectors;

    auto rooms = editableWorld.rooms;
    
    // Iterate over room to remove convexity
    for (auto& room : rooms)
    {
        for(int i = 0; i < room.cornersIndexes.size(); ++i)
        {
            int c1 = room.cornersIndexes[i];
            int i2 = (i + 1) % room.cornersIndexes.size();
            int c2 = room.cornersIndexes[i2];
            int i3 = (i + 2) % room.cornersIndexes.size();
            int c3 = room.cornersIndexes[i3];
            
            float a = editableWorld.corners[c1].x - editableWorld.corners[c2].x;
            float b = editableWorld.corners[c1].y - editableWorld.corners[c2].y;
            float c = editableWorld.corners[c3].x - editableWorld.corners[c2].x;
            float d = editableWorld.corners[c3].y - editableWorld.corners[c2].y;

            float atanA = atan2(a, b);
            float atanB = atan2(c, d);

            float angle = atanA - atanB;
            if (angle > M_PI)
            {
                angle -= 2 * M_PI;
            }
            else if (angle < -M_PI)
            {
                angle += 2 * M_PI;
            }

            if (angle > 0) // Convexity detected
            {
                std::vector<int> newCorners;
                int j = i2;
                while (j != i)
                {
                    newCorners.push_back(room.cornersIndexes[j]);
                    j = (j + 1) % room.cornersIndexes.size();
                }
                room.cornersIndexes = {room.cornersIndexes[(i-1)% room.cornersIndexes.size()], room.cornersIndexes[i], room.cornersIndexes[i2]};
                rooms.emplace_back(room.ceil, room.floor, newCorners);
            }
        }
    }
    
    // Iterate over rooms to create sectors
    for (auto& room : rooms)
    {
        // Create a sector
        sectors.push_back({room.floor, room.ceil, walls.size(), room.cornersIndexes.size()});
        // Iterate over corners
        for (int i = 0; i < room.cornersIndexes.size(); ++i)
        {
            int c1 = room.cornersIndexes[i];
            int c2 = room.cornersIndexes[(i + 1) % room.cornersIndexes.size()];
            size_t portal = 0;
            
            // Get wall key in order to check if it already exists
            std::tuple key(editableWorld.corners[c1].x, editableWorld.corners[c1].y, editableWorld.corners[c2].x, editableWorld.corners[c2].y);
            if (get<0>(key) > get<2>(key))
            {
                key = {get<2>(key), get<3>(key), get<0>(key), get<1>(key)};
            }else if(get<0>(key) == get<2>(key) && get<1>(key) > get<3>(key))
            {
                key = {get<2>(key), get<3>(key), get<0>(key), get<1>(key)};
            }
            
            bool found = false;
            // Check if wall already exists
            if (wallSectors.contains(key))
            {
                Wall& wall = walls.at(get<0>(wallSectors[key]));
                wall.portal = sectors.size();
                portal = get<1>(wallSectors[key]);
                found = true;
            }
            // Create wall
            walls.push_back({editableWorld.corners[c1].x, editableWorld.corners[c1].y, editableWorld.corners[c2].x, editableWorld.corners[c2].y, portal});
            if (!found)
            {
                wallSectors[key] = {walls.size()-1, sectors.size()};
            }
        }
    }
    Sector* sectorsArr = new Sector[sectors.size()];
    Wall* wallsArr = new Wall[walls.size()];
    for (size_t i = 0; i < sectors.size(); ++i)
    {
        sectorsArr[i] = sectors[i];
    }
    for (size_t i = 0; i < walls.size(); ++i)
    {
        wallsArr[i] = walls[i];
    }
    outWorld.SetSectors(sectorsArr, sectors.size());
    outWorld.SetWalls(wallsArr, walls.size());
}
