#include "CornerFeature.h"

#include "imgui.h"

void CornerFeature::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    worldEditorRenderer.SetDrawingColor(GetCurrentColor());
    EditableWorld::Corner& corner = worldEditorRenderer.GetWorld().corners[cornerIndex];
    worldEditorRenderer.DrawWorldPoint(corner.x, corner.y,3);
}

bool CornerFeature::CanSelect(EditableWorld& editableWorld, float x, float y)
{
    EditableWorld::Corner& corner = editableWorld.corners[cornerIndex];

    return (x - corner.x) * (x - corner.x) + (y - corner.y) * (y - corner.y) < selectionRadius * selectionRadius;
}

void CornerFeature::Drag(EditableWorld& world, float dx, float dy)
{
    WorldEditor::MoveCorner(world, cornerIndex, dx, dy);
}

void CornerFeature::RenderGui(EditableWorld& editableWorld)
{
    EditableWorld::Corner& corner = editableWorld.corners[cornerIndex];
    float position[2] = {corner.x, corner.y};
    ImGui::DragFloat2("Position", position, 0.01f);
    corner.x = position[0];
    corner.y = position[1];
}
