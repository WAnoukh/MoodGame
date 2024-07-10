#pragma once
#include "EditableWorld.h"

class WorldEditorRenderer
{
public:
    WorldEditorRenderer(EditableWorld& world) : world(&world) {};
    
    void RenderWalls(int width, int height, unsigned char* outData);
    EditableWorld& GetWorld() { return *world; }
    void WorldToView(float x, float y, float& outX, float& outY);
    void ViewToWorld(float x, float y, float& outX, float& outY);
    void VectorViewToWorld(float dx, float dy, float& outDx, float& outDy);
    void SetVerticalSize(float size);
    void SetCamera(float x, float y, float angle);
    void SetCameraX(float x);
    void SetCameraY(float y);
    void SetCameraAngle(float angle);

    int GetFrameWidth() const { return frameWidth; }
    int GetFrameHeight() const { return frameHeight; }
    
    //Drawing methods
    void DrawGrid();
    void NewFrame(int width, int height, unsigned char* outData);
    void SetDrawingColor(unsigned char r, unsigned char g, unsigned char b);
    void SetDrawingColor(const unsigned char* color);
    void DrawWorldLine(float x1, float y1, float x2, float y2);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawWorldPoint(float x, float y, float size = 3);
    void DrawPixel(int x, int y);
    
    float GetCamVerticalSize() const;
private:
    EditableWorld* world;
    float camX = 3;
    float camY = 3;
    float camAngle = 0;
    float camVerticalSize = 10;

    int frameWidth = 0;
    int frameHeight = 0;
    unsigned char* frameData = nullptr;

    unsigned char drawingColor[3] = { 255, 0, 0 };
    
    unsigned char strongUnitGridColor[3] = { 100, 100, 100 };
    unsigned char weakUnitGridColor[3] = { 20, 20, 20 };
    int gridSubdivisions = 10;
};
