#pragma once
#include <list>

struct EditableWorld
{
    struct Corner
    {
        float x;
        float y;
    };

    struct Room
    {
        float ceil = 1.0f;
        float floor = 0.0f;
        std::list<Corner*> corners;
    };
    std::list<Room> rooms;
    std::list<Corner> corners;
};
