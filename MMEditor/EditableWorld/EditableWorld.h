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
    
    std::vector<Room> rooms;
    std::vector<Corner> corners;
    std::tuple<float, float, float> playerSpawnPos = {0.0f, 0.0f, 0.0f};
};