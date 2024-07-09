#include "WallFeature.h"

#include "imgui.h"

void WallFeature::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    worldEditorRenderer.SetDrawingColor(0, 255, 0);
    EditableWorld::Corner& corner1 = worldEditorRenderer.GetWorld().corners[cornerIndex1];
    worldEditorRenderer.SetDrawingColor(GetCurrentColor());
    EditableWorld::Corner& corner2 = worldEditorRenderer.GetWorld().corners[cornerIndex2];
    worldEditorRenderer.DrawWorldLine(corner1.x, corner1.y, corner2.x, corner2.y);
    
}

bool WallFeature::CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y)
{
    EditableWorld::Corner& corner1 = worldEditorRenderer.GetWorld().corners[cornerIndex1];
    EditableWorld::Corner& corner2 = worldEditorRenderer.GetWorld().corners[cornerIndex2];
    float xNorm =  corner2.y - corner1.y;
    float yNorm =  corner1.x - corner2.x;
    float wallNorm = sqrt(xNorm * xNorm + yNorm * yNorm);
    xNorm /= wallNorm;
    yNorm /= wallNorm;
    float nDist =  (x-corner1.x) * xNorm + (y-corner1.y) * yNorm;
    float pDist = ((x-corner1.x) * (corner2.x - corner1.x) + (y-corner1.y) * (corner2.y - corner1.y));
    if (pDist < 0)
    {
        return false;
    }
    if (pDist > (corner2.x - corner1.x) * (corner2.x - corner1.x) + (corner2.y - corner1.y) * (corner2.y - corner1.y))
    {
        return false;
    }
    
    return abs(nDist) < selectionRadius;
}

void WallFeature::Drag(WorldEditor& worldEditor, float dx, float dy)
{
    
    worldEditor.MoveCorner(cornerIndex1, dx, dy);
    worldEditor.MoveCorner(cornerIndex2, dx, dy);
}

void WallFeature::RenderGui(EditableWorld& editableWorld)
{
}
