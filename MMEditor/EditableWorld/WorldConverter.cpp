#include "WorldConverter.h"
#include "EditableWorld.h"
#include "../../src/World/World.h"
#include <map>
#include <list>
#include <tuple>
using namespace std;

EditableWorld WorldConverter::ConvertWorld(World* world)
{
    EditableWorld editableWorld;
    map<tuple<float, float>, EditableWorld::Corner> cornersMap;
    size_t sectorIndex;
    for (sectorIndex = 0; sectorIndex < world->GetSectorsCount(); ++sectorIndex)
    {
        Sector& sector = *world->GetSector(sectorIndex);
        editableWorld.rooms.emplace_back();
        EditableWorld::Room& room = editableWorld.rooms.back();
        room.ceil = sector.ceil;
        room.floor = sector.floor;
        for (size_t wallIndex = 0; wallIndex < sector.numWalls; ++wallIndex)
        {
            Wall& wall = *world->GetWall(sector.firstWall + wallIndex);
            auto addNode = [&cornersMap,&room](float x, float y)
            {
                tuple<float, float> c1Key(x, y);
                if(!cornersMap.contains(c1Key))
                {
                    EditableWorld::Corner corner;
                    corner.x = x;
                    corner.y = y;
                    cornersMap[c1Key] = corner;
                    room.corners.push_back(&cornersMap[c1Key]);
                }
            };
            addNode(wall.x1, wall.y1);
            addNode(wall.x2, wall.y2);
        }
    }
}
