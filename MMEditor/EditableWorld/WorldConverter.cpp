#include "WorldConverter.h"
#include "EditableWorld.h"
#include "../../src/World/World.h"
#include <map>
#include <list>
#include <tuple>
using namespace std;

void WorldConverter::ConvertWorld(EditableWorld& editableWorld, World& world)
{
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