#pragma once
#include <vector>

struct EditableWorld
{
    EditableWorld() = default;
    struct Corner
    {
        float x;
        float y;
    };

    struct Room
    {
        float ceil = 1.0f;
        float floor = 0.0f;
        std::vector<int> cornersIndexes;
    };
    EditableWorld(EditableWorld const&) = delete;
    EditableWorld& operator=(EditableWorld const&) = delete;
    
    std::vector<Room> rooms;
    std::vector<Corner> corners;
};
