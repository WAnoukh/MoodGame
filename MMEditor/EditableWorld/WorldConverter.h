#pragma once
#include "EditableWorld.h"

class World;

class WorldConverter
{
public:
    static EditableWorld ConvertWorld(World* world);
};
