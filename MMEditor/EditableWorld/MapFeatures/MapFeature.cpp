#include "MapFeature.h"

int MapFeature::GetDrawPriority() const
{
    return drawPriority;
}

int MapFeature::GetSelectPriority() const
{
    return selectPriority;
}

void MapFeature::SetState(State inState)
{
    state = inState;
}

MapFeature::State MapFeature::GetState() const
{
    return state;
}

const unsigned char* MapFeature::GetCurrentColor() const
{
    switch (state)
    {
        case Hovered:
            return hoveredColor;
        case Selected:
            return selectedColor;
        default:
        case Normal:
            return normalColor;
    }
}

void MapFeature::SetNormalColor(unsigned char* color)
{
    normalColor[0] = color[0];
    normalColor[1] = color[1];
    normalColor[2] = color[2];
}

void MapFeature::SetNormalColor(unsigned char r, unsigned char g, unsigned char b)
{
    normalColor[0] = r;
    normalColor[1] = g;
    normalColor[2] = b;
}

void MapFeature::SetHoveredColor(unsigned char* color)
{
    hoveredColor[0] = color[0];
    hoveredColor[1] = color[1];
    hoveredColor[2] = color[2];
}

void MapFeature::SetHoveredColor(unsigned char r, unsigned char g, unsigned char b)
{
    hoveredColor[0] = r;
    hoveredColor[1] = g;
    hoveredColor[2] = b;
}

void MapFeature::SetSelectedColor(unsigned char* color)
{
    selectedColor[0] = color[0];
    selectedColor[1] = color[1];
    selectedColor[2] = color[2];
}

void MapFeature::SetSelectedColor(unsigned char r, unsigned char g, unsigned char b)
{
    selectedColor[0] = r;
    selectedColor[1] = g;
    selectedColor[2] = b;
}
