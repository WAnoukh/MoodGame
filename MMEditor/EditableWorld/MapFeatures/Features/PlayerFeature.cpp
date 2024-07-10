#include "PlayerFeature.h"

#include <corecrt_math_defines.h>

#include "imgui.h"

void PlayerFeature::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    worldEditorRenderer.SetDrawingColor(GetCurrentColor());
    auto [x, y, orientation] = worldEditorRenderer.GetWorld().playerSpawnPos;
    worldEditorRenderer.DrawWorldPoint(x, y,6);
    worldEditorRenderer.SetDrawingColor(255,255,255);
    float screenX, screenY;
    worldEditorRenderer.WorldToView(x, y, screenX, screenY);
    worldEditorRenderer.DrawLine(screenX, screenY, screenX - sin(orientation) * 10, screenY - cos(orientation) * 10);
}

bool PlayerFeature::CanSelect(EditableWorld& editableWorld, float x, float y)
{
    auto [px, py, orientation] = editableWorld.playerSpawnPos;

    return (x - px) * (x - px) + (y - py) * (y - py) < selectionRadius * selectionRadius;
}

void PlayerFeature::Drag(EditableWorld& world, float dx, float dy)
{
    auto& [x, y, orientation] = world.playerSpawnPos;
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


