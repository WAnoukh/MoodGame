#pragma once
#include <list>

#include "EditableWorld.h"

class WorldEditor
{
public:
    static void SetCornerPosition(EditableWorld& world, int cornerIndex, float x, float y);
    static void MoveCorner(EditableWorld& world, int cornerIndex, float dx, float dy);
    static int Extrude(EditableWorld& world, int cornerIndex1, int cornerIndex2, int& outExtrudedCorner1, int& outExtrudedCorner2, bool
                       separateRooms = true);
    static void CorrectRoomCornersOrder(EditableWorld& world, int roomIndex);
};
