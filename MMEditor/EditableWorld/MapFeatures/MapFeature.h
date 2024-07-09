#pragma once
#include "../WorldEditorRenderer.h"
#include "../WorldEditor.h"

#define WALLDRAWPRIORITY 1
#define WALLSELECTPRIORITY 1
#define ROOMDRAWPRIORITY 2
#define ROOMSELECTPRIORITY 2
#define CORNERDRAWPRIORITY 3
#define CORNERSELECTPRIORITY 3


class MapFeature
{
public:
    enum State
    {
        Normal,
        Hovered,
        Selected
    };
    virtual ~MapFeature() = default;
    virtual void Draw(WorldEditorRenderer& worldEditorRenderer) = 0;
    virtual bool CanSelect(WorldEditorRenderer& worldEditorRenderer, float x, float y) = 0;
    virtual void Drag(WorldEditor worldEditor, float dx, float dy) = 0;
    virtual void RenderGui(EditableWorld& editableWorld) {};

    int GetDrawPriority() const;
    int GetSelectPriority() const;
    void SetState(State inState);
    State GetState() const;
    const unsigned char* GetCurrentColor() const;
protected:
    int drawPriority = 0;
    int selectPriority = 0;
    State state = Normal;


    unsigned char normalColor[3] = {0, 0, 255};
    unsigned char hoveredColor[3] = {5, 209, 255};
    unsigned char selectedColor[3] = {255, 255, 0};
};
