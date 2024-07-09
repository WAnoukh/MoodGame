#pragma once
#include "../MapFeature.h"

class RoomFeature : public MapFeature
{
public:
    explicit RoomFeature(int roomIndex): roomIndex(roomIndex)
    {
        drawPriority = ROOMDRAWPRIORITY;
        selectPriority = ROOMSELECTPRIORITY;
    }
    void Draw(WorldEditorRenderer& worldEditorRenderer) override;
    bool CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y) override;
    void Drag(WorldEditor& worldEditor, float dx, float dy) override;

    void RenderGui(EditableWorld& editableWorld) override;

private:
    int roomIndex;
};
