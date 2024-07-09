#pragma once
#include "../WorldEditorRenderer.h"
#include "../WorldEditor.h"

#define ROOMDRAWPRIORITY 1
#define ROOMSELECTPRIORITY 1
#define WALLDRAWPRIORITY 2
#define WALLSELECTPRIORITY 2
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
    virtual void Drag(WorldEditor& worldEditor, float dx, float dy) = 0;
    virtual void RenderGui(EditableWorld& editableWorld) {};

    int GetDrawPriority() const;
    int GetSelectPriority() const;
    void SetState(State inState);
    State GetState() const;
    const unsigned char* GetCurrentColor() const;

    void SetNormalColor(unsigned char* color);
    void SetNormalColor(unsigned char r, unsigned char g, unsigned char b);
    void SetHoveredColor(unsigned char* color);
    void SetHoveredColor(unsigned char r, unsigned char g, unsigned char b);
    void SetSelectedColor(unsigned char* color);
    void SetSelectedColor(unsigned char r, unsigned char g, unsigned char b);
protected:
    int drawPriority = 0;
    int selectPriority = 0;
    State state = Normal;


    unsigned char normalColor[3] = {0, 0, 255};
    unsigned char hoveredColor[3] = {5, 209, 255};
    unsigned char selectedColor[3] = {255, 255, 0};
};
