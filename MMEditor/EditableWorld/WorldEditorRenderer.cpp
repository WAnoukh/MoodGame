#include "WorldEditorRenderer.h"

#include "EditableWorld.h"
#include <cmath>
#include "../../src/Texture/TextureDrawing.h"

void WorldEditorRenderer::RenderWalls(int width, int height, unsigned char* outData)
{
    for(auto& r : world->rooms)
    {
        for(int cornerIndex = 0; cornerIndex < r.cornersIndexes.size(); ++cornerIndex)
        {
            float x1, y1, x2, y2;
            const auto c1 = r.cornersIndexes[cornerIndex];
            const auto c2 = r.cornersIndexes[(cornerIndex + 1) % r.cornersIndexes.size()];
            const EditableWorld::Corner& corner1 = world->corners[c1];
            const EditableWorld::Corner& corner2 = world->corners[c2];
            WorldToView(corner1.x, corner1.y, x1, y1, width, height);
            WorldToView(corner2.x, corner2.y, x2, y2, width, height);
            TextureDrawing::DrawLine(x1, y1, x2, y2, outData, width, height, 0x00, 0xFF, 0x00);
        }
    }
}

void WorldEditorRenderer::WorldToView(float x, float y, float& outX, float& outY, int width, int height)
{
    const float ratio = static_cast<float>(height) / static_cast<float>(width);
    const float cX = x - camX;
    const float cY = y - camY;
    outX = ((cX * cos(camAngle) - cY * sin(camAngle)) * ratio / camVerticalSize + 0.5f) * width;
    outY = (0.5f - (cX * sin(camAngle) + cY * cos(camAngle)) / camVerticalSize) * height;
}

void WorldEditorRenderer::ViewToWorld(float x, float y, float& outX, float& outY, int width, int height)
{
    const float ratio = (float)height / (float)width;
    const float cX = (x / width - 0.5f) / ratio * camVerticalSize;
    const float cY = (0.5f - y / height) * camVerticalSize;
    outX = cX * cos(-camAngle) - cY * sin(-camAngle) + camX;
    outY = cX * sin(-camAngle) + cY * cos(-camAngle) + camY;
}

void WorldEditorRenderer::DeltaViewToWorld(float dx, float dy, float& outDx, float& outDy, int width, int height)
{
    const float ratio = (float)height / (float)width;
    const float cX = (dx / width) / ratio * camVerticalSize;
    const float cY = (dy / height) * camVerticalSize;
    outDx = cX * cos(-camAngle) - cY * sin(-camAngle);
    outDy = cX * sin(-camAngle) + cY * cos(-camAngle);
    
}

void WorldEditorRenderer::SetVerticalSize(float size)
{
    camVerticalSize = size;
}

void WorldEditorRenderer::SetCamera(float x, float y, float angle)
{
    SetCameraX(x);
    SetCameraY(y);
    SetCameraAngle(angle);
}

void WorldEditorRenderer::SetCameraX(float x)
{
    camX = x;
}

void WorldEditorRenderer::SetCameraY(float y)
{
    camY = y;
}

void WorldEditorRenderer::SetCameraAngle(float angle)
{
    camAngle = angle;
}

float WorldEditorRenderer::GetCamVerticalSize() const
{
    return camVerticalSize;
}
