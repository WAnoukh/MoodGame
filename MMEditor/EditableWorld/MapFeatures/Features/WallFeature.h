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
    bool CanSelect(EditableWorld& editableWorld, float x, float y) override;
    void Drag(EditableWorld& world, float dx, float dy) override;
    void RenderGui(EditableWorld& editableWorld) override;

    int GetCorner1() const { return cornerIndex1; }
    int GetCorner2() const { return cornerIndex2; }
private:
    int cornerIndex1;
    int cornerIndex2;
    float selectionRadius = 0.1f;
};
