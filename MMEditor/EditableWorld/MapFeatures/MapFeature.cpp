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
