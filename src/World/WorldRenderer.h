#pragma once
#include "../Texture/ByteTexture.h"

class World;

class WorldRenderer
{
public:
    WorldRenderer(World& worldToRender): world(&worldToRender) {}

    void Render(int width, int height, unsigned char* outData);

    float GetCamX() const { return camX; }
    float GetCamY() const { return camY; }
    float GetCamZ() const { return camZ; }
    float GetCamAngle() const { return camAngle; }
    float GetCamVerticalAngle() const { return camVerticalAngle; }
    void SetCamX(float inX) { camX = inX; }
    void SetCamY(float inY) { camY = inY; }
    void SetCamZ(float inZ) { camZ = inZ; }
    void SetCamAngle(float inAngle) { camAngle = inAngle; }
    void SetCamVerticalAngle(float inAngle) { camVerticalAngle = inAngle; }
    
private:
    World* world = nullptr;
    ByteTexture* wallTexture = ByteTexture::LoadTexture("../res/Textures/GRAY1.png");
    ByteTexture* floorTexture = ByteTexture::LoadTexture("../res/Textures/GRAY1.png");
    ByteTexture* ceilingTexture = ByteTexture::LoadTexture("../res/Textures/grid.png");
    float camX = 3;
    float camY = 3;
    float camZ = 1.0f;
    float camAngle = 0;
    float camVerticalAngle = 0;
    float HFOV = 90;
};
