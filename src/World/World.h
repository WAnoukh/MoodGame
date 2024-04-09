#pragma once

struct Wall
{
    float x1, y1, x2, y2;
    size_t portal;
};

struct Sector
{
    float floor, ceil;
    size_t firstWall;
    size_t numWalls;
};

bool PointInside(float x, float y, const Wall& wall);

class World
{
public:
    ~World();
    void SetSectors(Sector* inSectors, size_t inNum);
    void SetWalls(Wall* inWalls, size_t inNum);

    bool PointInsideSector(float x, float y, size_t sectorIndex) const;
    size_t GetSectorAtPos(float x, float y) const;
    Sector* GetSector(size_t index) const;
    Wall* GetWall(size_t index) const;
    size_t GetSectorsCount() const;
    size_t GetWallsCount() const;
private:
    Sector* sectors = nullptr;
    Wall* walls = nullptr;
    size_t numSectors = 0;
    size_t numWalls = 0;
};
