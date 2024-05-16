#include "WorldConverter.h"
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

void WorldConverter::ConvertEditableWorld(World& outWorld, EditableWorld& editableWorld)
{
    std::vector<Wall> walls;
    std::map<tuple<float, float, float, float>, tuple<int, size_t>> wallSectors;
    std::vector<Sector> sectors;
    for (auto& room : editableWorld.rooms)
    {
        sectors.push_back({room.floor, room.ceil, walls.size(), room.cornersIndexes.size()});
        for (int i = 0; i < room.cornersIndexes.size(); ++i)
        {
            int c1 = room.cornersIndexes[i];
            int c2 = room.cornersIndexes[(i + 1) % room.cornersIndexes.size()];
            size_t portal = 0;
            std::tuple key(editableWorld.corners[c1].x, editableWorld.corners[c1].y, editableWorld.corners[c2].x, editableWorld.corners[c2].y);
            if (get<0>(key) > get<2>(key))
            {
                key = {get<2>(key), get<3>(key), get<0>(key), get<1>(key)};
            }else if(get<0>(key) == get<2>(key) && get<1>(key) > get<3>(key))
            {
                key = {get<2>(key), get<3>(key), get<0>(key), get<1>(key)};
            }
            
            bool found = false;
            if (wallSectors.contains(key))
            {
                Wall& wall = walls.at(get<0>(wallSectors[key]));
                wall.portal = sectors.size();
                portal = get<1>(wallSectors[key]);
                found = true;
            }
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
