#include "WorldEditorRenderer.h"

#include "EditableWorld.h"
#include <cmath>
#include "../../src/Texture/TextureDrawing.h"

void WorldEditorRenderer::RenderWalls(int frameWidth, int frameHeight, unsigned char* outData)
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
            WorldToView(corner1.x, corner1.y, x1, y1);
            WorldToView(corner2.x, corner2.y, x2, y2);
            TextureDrawing::DrawLine(x1, y1, x2, y2, outData, frameWidth, frameHeight, 0x00, 0xFF, 0x00);
        }
    }
}

void WorldEditorRenderer::WorldToView(float x, float y, float& outX, float& outY)
{
    const float ratio = static_cast<float>(frameHeight) / static_cast<float>(frameWidth);
    const float cX = x - camX;
    const float cY = y - camY;
    outX = ((cX * cos(camAngle) - cY * sin(camAngle)) * ratio / camVerticalSize + 0.5f) * frameWidth;
    outY = (0.5f - (cX * sin(camAngle) + cY * cos(camAngle)) / camVerticalSize) * frameHeight;
}

void WorldEditorRenderer::ViewToWorld(float x, float y, float& outX, float& outY)
{
    const float ratio = (float)frameHeight / (float)frameWidth;
    const float cX = (x / frameWidth - 0.5f) / ratio * camVerticalSize;
    const float cY = (0.5f - y / frameHeight) * camVerticalSize;
    outX = cX * cos(-camAngle) - cY * sin(-camAngle) + camX;
    outY = cX * sin(-camAngle) + cY * cos(-camAngle) + camY;
}

void WorldEditorRenderer::VectorViewToWorld(float dx, float dy, float& outDx, float& outDy)
{
    const float ratio = (float)frameHeight / (float)frameWidth;
    const float cX = (dx / frameWidth) / ratio * camVerticalSize;
    const float cY = (dy / frameHeight) * camVerticalSize;
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

void WorldEditorRenderer::NewFrame(int width, int height, unsigned char* outData)
{
    frameData = outData;
    frameWidth = width;
    frameHeight = height;
}

void WorldEditorRenderer::SetDrawingColor(unsigned char r, unsigned char g, unsigned char b)
{
    drawingColor[0] = r;
    drawingColor[1] = g;
    drawingColor[2] = b;
}

void WorldEditorRenderer::SetDrawingColor(const unsigned char* color)
{
    drawingColor[0] = color[0];
    drawingColor[1] = color[1];
    drawingColor[2] = color[2];
}

void WorldEditorRenderer::DrawWorldLine(float x1, float y1, float x2, float y2)
{
    float newX1, newY1, newX2, newY2;
    WorldToView(x1, y1, newX1, newY1);
    WorldToView(x2, y2, newX2, newY2);
    TextureDrawing::DrawLine(newX1, newY1, newX2, newY2, frameData, frameWidth, frameHeight, drawingColor[0], drawingColor[1], drawingColor[2]);
}

void WorldEditorRenderer::DrawWorldPoint(float x, float y, float size)
{
    float newX, newY;
    WorldToView(x, y, newX, newY);
    TextureDrawing::DrawCircle(newX, newY, size, frameData, frameWidth, frameHeight, drawingColor[0], drawingColor[1], drawingColor[2]);
}

void WorldEditorRenderer::DrawPixel(int x, int y)
{
    if (x < 0 || y < 0 || x >= frameWidth || y >= frameHeight)
    {
        return;
    }
    TextureDrawing::DrawPixel(x, y, frameData, frameWidth, drawingColor[0], drawingColor[1], drawingColor[2]);
}

float WorldEditorRenderer::GetCamVerticalSize() const
{
    return camVerticalSize;
}
