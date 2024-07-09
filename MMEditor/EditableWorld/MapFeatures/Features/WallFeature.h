#pragma once
#include "../MapFeature.h"

class WallFeature : public MapFeature
{
public:
    WallFeature(int cornerIndex1, int cornerIndex2): cornerIndex1(cornerIndex1),cornerIndex2(cornerIndex2)
    {
        drawPriority = WALLDRAWPRIORITY;
        selectPriority = WALLSELECTPRIORITY;
    }
    void Draw(WorldEditorRenderer& worldEditorRenderer) override;
    bool CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y) override;
    void Drag(WorldEditor worldEditor, float dx, float dy) override;

private:
    int cornerIndex1;
    int cornerIndex2;
    float selectionRadius = 0.1f;
};
