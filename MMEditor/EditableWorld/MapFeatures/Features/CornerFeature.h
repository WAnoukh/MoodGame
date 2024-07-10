#pragma once
#include "../MapFeature.h"



class CornerFeature : public MapFeature
{
public:
    explicit CornerFeature(int cornerIndex) : cornerIndex(cornerIndex)
    {
        drawPriority = CORNERDRAWPRIORITY;
        selectPriority = CORNERSELECTPRIORITY;
        SetNormalColor(255,0,0);
        SetHoveredColor(255,170,0);
    }
    auto Draw(WorldEditorRenderer& worldEditorRenderer) -> void override;
    bool CanSelect(EditableWorld& editableWorld, float x, float y) override;
    void Drag(EditableWorld& world, float dx, float dy) override;
    void RenderGui(EditableWorld& editableWorld) override;

private:
    const float selectionRadius = 0.2f;
    int cornerIndex;
};
