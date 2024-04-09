#pragma once

class World;

class MapRenderer
{
public:
    MapRenderer(World& worldToRender): world(&worldToRender) {}

    void RenderWalls(int width, int height, unsigned char* outData);
    void WorldToScreen(float x, float y, float& outX, float& outY, int width, int height);

private:
    World* world = nullptr;
    float camX = 3;
    float camY = 3;
    float camAngle = 0;
    float camVerticalSize = 10;
};
