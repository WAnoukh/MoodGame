#pragma once
#include "EditableWorld.h"

class WorldEditorRenderer
{
public:
    WorldEditorRenderer(EditableWorld& world) : world(&world) {};
    
    void RenderWalls(int width, int height, unsigned char* outData);
    void WorldToView(float x, float y, float& outX, float& outY, int width, int height);
    void ViewToWorld(float x, float y, float& outX, float& outY, int width, int height);
    void DeltaViewToWorld(float dx, float dy, float& outDx, float& outDy, int width, int height);
    void SetVerticalSize(float size);
    void SetCamera(float x, float y, float angle);
    void SetCameraX(float x);
    void SetCameraY(float y);
    void SetCameraAngle(float angle);

    float GetCamVerticalSize() const;
private:
    EditableWorld* world;
    float camX = 3;
    float camY = 3;
    float camAngle = 0;
    float camVerticalSize = 10;
};
