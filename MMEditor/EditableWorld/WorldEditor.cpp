#include "WorldEditor.h"

#include <corecrt_math_defines.h>

void WorldEditor::SetCornerPosition(EditableWorld& world, int cornerIndex, float x, float y)
{
    auto& corner = world.corners[cornerIndex];
    corner.x = x;
    corner.y = y;
}

void WorldEditor::MoveCorner(EditableWorld& world, int cornerIndex, float dx, float dy)
{
    auto& corner = world.corners[cornerIndex];
    corner.x += dx;
    corner.y += dy;
}

int WorldEditor::Extrude(EditableWorld& world, int cornerIndex1, int cornerIndex2, int& outExtrudedCorner1, int& outExtrudedCorner2, bool
                         separateRooms)
{
    int foundOccurences = 0;
    EditableWorld::Room* foundRoom = nullptr;
    int foundCornerIndex = 0;
    int curCornerIndex1, curCornerIndex2;
    for(auto& room : world.rooms)
    {
        for(int cornerIndex = 0; cornerIndex < room.cornersIndexes.size(); ++cornerIndex)
        {
            curCornerIndex1 = room.cornersIndexes[cornerIndex];
            curCornerIndex2 = room.cornersIndexes[(cornerIndex + 1) % room.cornersIndexes.size()];
            bool swapped = curCornerIndex1 > curCornerIndex2;
            if (swapped)
                std::swap(curCornerIndex1, curCornerIndex2);
            if(curCornerIndex1 == cornerIndex1)
            {
                if(curCornerIndex2 == cornerIndex2)
                {
                    ++foundOccurences;
                    foundRoom = &room;
                    foundCornerIndex = cornerIndex;
                    if (swapped)
                        std::swap(cornerIndex1, cornerIndex2);
                    break;
                }
            }
        }
    }
    if (foundOccurences == 0)
        return -1;
    if (foundOccurences > 1)
        return -2;
    EditableWorld::Corner newCorner1 = world.corners[cornerIndex1];
    EditableWorld::Corner newCorner2 = world.corners[cornerIndex2];
    world.corners.push_back(newCorner1);
    world.corners.push_back(newCorner2);
    outExtrudedCorner1 = world.corners.size() - 2;
    outExtrudedCorner2 = world.corners.size() - 1;
    
    if (separateRooms)
    {
        EditableWorld::Room newRoom;
        newRoom.ceil = foundRoom->ceil;
        newRoom.floor = foundRoom->floor;
        newRoom.cornersIndexes.push_back(cornerIndex2);
        newRoom.cornersIndexes.push_back(cornerIndex1);
        newRoom.cornersIndexes.push_back(outExtrudedCorner1);
        newRoom.cornersIndexes.push_back(outExtrudedCorner2);
    
        world.rooms.push_back(newRoom); 
    }
    else
    {
        ++foundCornerIndex;
        foundCornerIndex %= foundRoom->cornersIndexes.size();
        foundRoom->cornersIndexes.insert(foundRoom->cornersIndexes.begin() + foundCornerIndex, outExtrudedCorner2);
        foundRoom->cornersIndexes.insert(foundRoom->cornersIndexes.begin() + foundCornerIndex, outExtrudedCorner1);
    }

}

void WorldEditor::CorrectRoomCornersOrder(EditableWorld& world, int roomIndex)
{
    EditableWorld::Room& room = world.rooms[roomIndex];
    int ClockTurn = 0;
    int AClockTurn = 0;
    for(int i = 0; i < room.cornersIndexes.size(); ++i)
    {
        int c1 = room.cornersIndexes[i];
        int i2 = (i + 1) % room.cornersIndexes.size();
        int c2 = room.cornersIndexes[i2];
        int i3 = (i + 2) % room.cornersIndexes.size();
        int c3 = room.cornersIndexes[i3];
        
        float a = world.corners[c1].x - world.corners[c2].x;
        float b = world.corners[c1].y - world.corners[c2].y;
        float c = world.corners[c3].x - world.corners[c2].x;
        float d = world.corners[c3].y - world.corners[c2].y;

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
            ++AClockTurn;
        }else if (angle < 0)
        {
            ++ClockTurn;
        }
    }
    if (AClockTurn > ClockTurn)
    {
        std::reverse(room.cornersIndexes.begin(), room.cornersIndexes.end());
    }
}

int WorldEditor::AddCorner(EditableWorld& world, int cornerIndex1, int cornerIndex2, float ratio)
{
    EditableWorld::Corner newCorner;
    newCorner.x = world.corners[cornerIndex1].x * (1 - ratio) + world.corners[cornerIndex2].x * ratio;
    newCorner.y = world.corners[cornerIndex1].y * (1 - ratio) + world.corners[cornerIndex2].y * ratio;
    world.corners.push_back(newCorner);
    for (auto& room : world.rooms)
    {
        for (int i = 0; i < room.cornersIndexes.size(); ++i)
        {
            int curCornerIndex1 = room.cornersIndexes[i];
            int curCornerIndex2 = room.cornersIndexes[(i + 1) % room.cornersIndexes.size()];
            if (curCornerIndex1 > curCornerIndex2)
                std::swap(curCornerIndex1, curCornerIndex2);
            if (curCornerIndex1 == cornerIndex1 && curCornerIndex2 == cornerIndex2)
            {
                room.cornersIndexes.insert(room.cornersIndexes.begin() + i + 1, world.corners.size() - 1);
            }
        }
    }
    return world.corners.size() - 1;
}

