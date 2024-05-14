#include "WorldEditor.h"

int WorldEditor::GetNearestCorner(const float x, const float y, const float maxDist) const
{
    int nearest = -1;
    float nearestDist = maxDist;
    for (int i = 0; i < world->corners.size(); i++)
    {
        float dx = world->corners[i].x - x;
        float dy = world->corners[i].y - y;
        float dist = dx * dx + dy * dy;
        if (dist < nearestDist)
        {
            nearest = i;    
            nearestDist = dist;
        }
    }
    return nearest;
}

void WorldEditor::GetCornerPosition(int cornerIndex, float& x, float& y)
{
    auto corner = world->corners[cornerIndex];
    x = corner.x;
    y = corner.y;
}

void WorldEditor::SetCornerPosition(int cornerIndex, float x, float y)
{
    auto& corner = world->corners[cornerIndex];
    corner.x = x;
    corner.y = y;
}

void WorldEditor::MoveCorner(int cornerIndex, float dx, float dy)
{
    auto& corner = world->corners[cornerIndex];
    corner.x += dx;
    corner.y += dy;
}

