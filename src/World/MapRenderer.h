#pragma once

class World;

class MapRenderer
{
public:
    MapRenderer(World& worldToRender): world(&worldToRender) {}

    void RenderWalls(int width, int height, unsigned char* outData);
    void WorldToView(float x, float y, float& outX, float& outY, int width, int height);
    void SetVerticalSize(float size);
    void SetCamera(float x, float y, float angle);
    void SetCameraX(float x);
    void SetCameraY(float y);
    void SetCameraAngle(float angle);

    float GetCamVerticalSize() const;

private:
    World* world = nullptr;
    float camX = 3;
    float camY = 3;
    float camAngle = 0;
    float camVerticalSize = 10;
};
