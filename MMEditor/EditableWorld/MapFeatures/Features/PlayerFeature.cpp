#include "PlayerFeature.h"

#include <corecrt_math_defines.h>

#include "imgui.h"

void PlayerFeature::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    worldEditorRenderer.SetDrawingColor(GetCurrentColor());
    auto [x, y, orientation] = worldEditorRenderer.GetWorld().playerSpawnPos;
    worldEditorRenderer.DrawWorldPoint(x, y,6);
    worldEditorRenderer.SetDrawingColor(255,255,255);
    worldEditorRenderer.DrawWorldLine(x, y, x - sin(orientation) * 0.2, y + cos(orientation) * 0.2);
}

bool PlayerFeature::CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y)
{
    auto [px, py, orientation] = worldEditorRenderer.GetWorld().playerSpawnPos;

    return (x - px) * (x - px) + (y - py) * (y - py) < selectionRadius * selectionRadius;
}

void PlayerFeature::Drag(WorldEditor& worldEditor, float dx, float dy)
{
    auto& [x, y, orientation] = worldEditor.GetWorld().playerSpawnPos;
    x += dx;
    y += dy;
}

void PlayerFeature::RenderGui(EditableWorld& editableWorld)
{
    auto [x, y, orientation] = editableWorld.playerSpawnPos;
    float position[2] = {x, y};
    ImGui::DragFloat2("Position", position, 0.01f);
    x = position[0];
    y = position[1];
    orientation =  360.0f * static_cast<float>(orientation) / (2.0f*static_cast<float>(M_PI));
    ImGui::DragFloat("Orientation", &orientation, 5.0f);
    editableWorld.playerSpawnPos = {x, y, orientation * (2*M_PI) / 360.0f};
}


