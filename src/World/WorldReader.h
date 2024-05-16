#pragma once
#include "World.h"
class WorldReader
{
public:
    WorldReader(World& worldToLoad, const char* filePath);
    int Load();
    int Save(const char* filePath);
    void SetPath(const char* filePath);
private:
    World* world;
    const char* path;
};