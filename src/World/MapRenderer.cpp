#include "MapRenderer.h"

#include "World.h"
#include <cmath>
#include "../Texture/TextureDrawing.h"

void MapRenderer::RenderWalls(int width, int height, unsigned char* outData)
{
    const float ratio = (float)height / (float)width;
    
    const int wallCount = world->GetWallsCount();
    for (int wallIndex = 0; wallIndex < wallCount; ++wallIndex)
    {
        float x1, y1, x2, y2;
        WorldToScreen(world->GetWall(wallIndex)->x1, world->GetWall(wallIndex)->y1, x1, y1, width, height);
        WorldToScreen(world->GetWall(wallIndex)->x2, world->GetWall(wallIndex)->y2, x2, y2, width, height);        
        const Wall* wall = world->GetWall(wallIndex);
        if (wall->portal)
        {
            TextureDrawing::DrawLine(x1, y1, x2, y2, outData, width, height, 0xFF, 0x00, 0x00);
        }
        else
        {
            TextureDrawing::DrawLine(x1, y1, x2, y2, outData, width, height, 0xFF, 0xFF, 0xFF);
        }
    }
}

void MapRenderer::WorldToScreen(float x, float y, float& outX, float& outY, int width, int height)
{
    const float ratio = (float)height / (float)width;
    const float cX = x - camX;
    const float cY = y - camY;
    outX = ((cX * cos(camAngle) - cY * sin(camAngle)) * ratio / camVerticalSize + 0.5f) * width;
    outY = (0.5f - (cX * sin(camAngle) + cY * cos(camAngle)) / camVerticalSize) * height;
}

void MapRenderer::SetVerticalSize(float size)
{
    camVerticalSize = size;
}

void MapRenderer::SetCamera(float x, float y, float angle)
{
    SetCameraX(x);
    SetCameraY(y);
    SetCameraAngle(angle);
}

void MapRenderer::SetCameraX(float x)
{
    camX = x;
}

void MapRenderer::SetCameraY(float y)
{
    camY = y;
}

void MapRenderer::SetCameraAngle(float angle)
{
    camAngle = angle;
}

float MapRenderer::GetCamVerticalSize() const
{
    return camVerticalSize;
}
