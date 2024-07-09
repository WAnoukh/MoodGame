#pragma once
#include "../MapFeature.h"

class PlayerFeature : public MapFeature
{
public:
    PlayerFeature()
    {
        drawPriority = PLAYERDRAWPRIORITY;
        selectPriority = PLAYERSELECTPRIORITY;
        SetNormalColor(255,0,0);
        SetHoveredColor(255,170,0);
    }
    void Draw(WorldEditorRenderer& worldEditorRenderer) override;
    bool CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y) override;
    void Drag(WorldEditor& worldEditor, float dx, float dy) override;
    void RenderGui(EditableWorld& editableWorld) override;
    float selectionRadius = 0.2f;
};