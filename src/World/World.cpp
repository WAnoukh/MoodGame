#include "World.h"
#define _USE_MATH_DEFINES  // NOLINT(clang-diagnostic-reserved-macro-identifier, bugprone-reserved-identifier)
#include <algorithm>
#include <cmath>
#include <corecrt_math_defines.h>

bool PointInside(float x, float y, const Wall& wall)
{
    float ax, ay, bx, by;
    ax = wall.x2 - wall.x1;
    ay = wall.y2 - wall.y1;
    bx = x - wall.x1;
    by = y - wall.y1;
    float ab = ax * by - ay * bx;
    float a = sqrt(ax * ax + ay * ay);
    float b = sqrt(bx * bx + by * by);
    float temp = std::min(std::max(ab/(a*b),-1.0f),1.0f);
    float angle = acos(temp) - M_PI/2;
    return angle >= 0;
}

World::~World()
{
    delete[] sectors;
    delete[] walls;
}

void World::SetSectors(Sector* inSectors, size_t inNum)
{
    delete[] sectors;
    
    sectors = inSectors;
    numSectors = inNum;
}

void World::SetWalls(Wall* inWalls, size_t inNum)
{
    delete[] walls;

    walls = inWalls;
    numWalls = inNum;
}

void World::SetPlayerSpawnPos(float x, float y, float orientation)
{
    playerSpawnPos = {x, y, orientation};
}

void World::SetPlayerSpawnPos(std::tuple<float, float, float> pos)
{
    playerSpawnPos = pos;
}

bool World::PointInsideSector(float x, float y, size_t sectorIndex) const
{
    const Sector& sector = sectors[sectorIndex-1];
    for (size_t i = 0; i < sector.numWalls; i++)
    {
        const Wall& wall = walls[sector.firstWall + i];
        if (!PointInside(x, y, wall))
        {
            return false;
        }
    }
    return true;
}

size_t World::GetSectorAtPos(float x, float y) const
{
    for (size_t i = 1; i <= numSectors; i++)
    {
        if (PointInsideSector(x, y, i))
        {
            return i;
        }
    }
    return 0;
}

Sector* World::GetSector(size_t index) const
{
    return &sectors[index-1];
}

Wall* World::GetWall(size_t index) const
{
    return &walls[index];
}

size_t World::GetSectorsCount() const
{
    return numSectors;
}

size_t World::GetWallsCount() const
{
    return numWalls;
}

std::tuple<float, float, float> World::GetPlayerSpawnPos() const
{
    return playerSpawnPos;
}
