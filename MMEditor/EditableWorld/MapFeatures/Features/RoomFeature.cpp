#include "RoomFeature.h"

#include <corecrt_math_defines.h>

#include "imgui.h"

void RoomFeature::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    EditableWorld::Room& room =worldEditorRenderer.GetWorld().rooms[roomIndex];
    int minx=10000, miny=10000, maxx= 0, maxy=0;
    for (size_t i = 0; i < room.cornersIndexes.size(); i++)
    {
        EditableWorld::Corner& corner1 = worldEditorRenderer.GetWorld().corners[room.cornersIndexes[i]];
        EditableWorld::Corner& corner2 = worldEditorRenderer.GetWorld().corners[room.cornersIndexes[(i+1)%room.cornersIndexes.size()]];
        float x1, y1, x2, y2;
        worldEditorRenderer.WorldToView(corner1.x, corner1.y, x1, y1);
        worldEditorRenderer.WorldToView(corner2.x, corner2.y, x2, y2);
        if (x1 < minx)
        {
            minx = x1;
        }
        if (y1 < miny)
        {
            miny = y1;
        }
        if (x1 > maxx)
        {
            maxx = x1;
        }
        if (y1 > maxy)
        {
            maxy = y1;
        }
        if (x2 < minx)
        {
            minx = x2;
        }
        if (y2 < miny)
        {
            miny = y2;
        }
        if (x2 > maxx)
        {
            maxx = x2;
        }
        if (y2 > maxy)
        {
            maxy = y2;
        }
    }
    maxx = std::min(maxx, worldEditorRenderer.GetFrameWidth());
    maxy = std::min(maxy, worldEditorRenderer.GetFrameHeight());
    minx = std::max(minx, 0);
    miny = std::max(miny, 0);
    worldEditorRenderer.SetDrawingColor(GetCurrentColor());
    for (int pixelX = minx; pixelX <= maxx; pixelX+=10)
    {
        for (int pixelY = miny; pixelY <= maxy; pixelY+=10)
        {
            float x, y;
            worldEditorRenderer.ViewToWorld(pixelX, pixelY, x, y);
            if (CanSelect(worldEditorRenderer.GetWorld(), x, y))
            {
                worldEditorRenderer.DrawPixel(pixelX, pixelY);
            }
        }
    }
}

bool RoomFeature::CanSelect(EditableWorld& editableWorld, float x, float y)
{
    EditableWorld::Room& room = editableWorld.rooms[roomIndex];
    for (size_t i = 0; i < room.cornersIndexes.size(); i++)
    {
        EditableWorld::Corner& corner1 = editableWorld.corners[room.cornersIndexes[i]];
        EditableWorld::Corner& corner2 = editableWorld.corners[room.cornersIndexes[(i+1)%room.cornersIndexes.size()]];
        float ax, ay, bx, by;
        ax = corner2.x - corner1.x;
        ay = corner2.y - corner1.y;
        bx = x - corner1.x;
        by = y - corner1.y;
        float ab = ax * by - ay * bx;
        float a = sqrt(ax * ax + ay * ay);
        float b = sqrt(bx * bx + by * by);
        float temp = std::min(std::max(ab/(a*b),-1.0f),1.0f);
        float angle = acos(temp) - M_PI/2;
        if (angle >= 0)
        {
            return false;
        }
    }
    return true;
}

void RoomFeature::Drag(EditableWorld& world, float dx, float dy)
{
    EditableWorld::Room& room =world.rooms[roomIndex];
    for (size_t i = 0; i < room.cornersIndexes.size(); i++)
    {
        WorldEditor::MoveCorner(world, room.cornersIndexes[i], dx, dy);
    }
}

void RoomFeature::RenderGui(EditableWorld& editableWorld)
{
    ImGui::InputFloat("Ceil", &editableWorld.rooms[roomIndex].ceil, 0.1f, 0.25f);
    ImGui::InputFloat("Floor", &editableWorld.rooms[roomIndex].floor, 0.1f, 0.25f);
}
