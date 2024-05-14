#pragma once
#include "EditableWorld.h"
#include "../../src/World/WorldReader.h"

class World;

class WorldConverter
{
public:
    static void ConvertWorld(EditableWorld& outEditableWorld, World& world);
    
};