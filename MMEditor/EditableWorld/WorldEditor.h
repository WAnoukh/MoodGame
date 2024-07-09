#pragma once
#include <list>

#include "EditableWorld.h"

class WorldEditor
{
public:
    WorldEditor(EditableWorld* world) : world(world) {}
    EditableWorld& GetWorld() { return *world; }
    int GetNearestCorner(float x, float y, float maxDist = 0.5f) const;
    void GetCornerPosition(int cornerIndex, float& x, float& y);
    void SetCornerPosition(int cornerIndex, float x, float y);
    void MoveCorner(int cornerIndex, float dx, float dy);
private:
    EditableWorld* world;
};
