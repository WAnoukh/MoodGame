#pragma once
#include "World.h"
class WorldReader
{
public:
    WorldReader(World& worldToLoad);
    int Load();
    int Save(const char* filePath);
    void SetPath(const char* filePath);
private:
    World* world;
    const char* path = nullptr;
};