#pragma once
#include "World.h"

class WorldReader
{
public:
    WorldReader(World& worldToLoad, const char* filePath);
    int Load();

private:
    World* world;
    const char* path;
};
