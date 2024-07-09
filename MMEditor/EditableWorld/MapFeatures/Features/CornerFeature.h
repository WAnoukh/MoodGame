#pragma once
#include "../MapFeature.h"

class CornerFeature : public MapFeature
{
public:
    explicit CornerFeature(int cornerIndex) : cornerIndex(cornerIndex)
    {
        drawPriority = CORNERDRAWPRIORITY;
        selectPriority = CORNERSELECTPRIORITY;
    }
    auto Draw(WorldEditorRenderer& worldEditorRenderer) -> void override;
    bool CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y) override;
    void Drag(WorldEditor worldEditor, float dx, float dy) override;
    void RenderGui(EditableWorld& editableWorld) override;

private:
    const float selectionRadius = 0.2f;
    int cornerIndex;
};
