#pragma once

namespace TextureDrawing
{
    inline void DrawPixel(int x1, int y1, unsigned char* data, int width, unsigned char r, unsigned char g, unsigned char b)
    {
        data[(x1 + y1 * width) * 3] = r;
        data[(x1 + y1 * width) * 3 + 1] = g;
        data[(x1 + y1 * width) * 3 + 2] = b;
    }
    void DrawLine(int x1, int y1, int x2, int y2, unsigned char* data, int width, int height,unsigned char r, unsigned char g, unsigned char b);
    void DrawRectangle(int x1, int y1, int x2, int y2, unsigned char* data, int width, int height, unsigned char r, unsigned char g, unsigned char b);
    void DrawCircle(int x, int y, int radius, unsigned char* data, int width, int height, unsigned char r, unsigned char g, unsigned char b);
}
