#include "TextureDrawing.h"

void TextureDrawing::DrawLine(int x1, int y1, int x2, int y2, unsigned char* data, int width, int height,
    unsigned char r, unsigned char g, unsigned char b)
{
    float slope = (float)(y2 - y1) / (x2 - x1);
    if ((slope > -1 && slope < 1) && x1 != x2)
    {
        int xStart;
        int xEnd;
        int yStart;
        if (x1 > x2)
        {
            xStart = x2;
            xEnd = x1;
            yStart = y2;
        }
        else
        {
            xStart = x1;
            xEnd = x2;
            yStart = y1;
        }
        for (int x = xStart; x <= xEnd; ++x)
        {
            int y = yStart + slope * (x - xStart);
            if (y < 0 || x < 0)
            {
                continue;
            }
            if (y >= height || x >= width)
            {
                continue;
            }
            DrawPixel(x, y, data, width, r, g, b);
        }
    }
    else
    {
        int xStart;
        int yStart;
        int yEnd;
        if (y1 > y2)
        {
            yStart = y2;
            yEnd = y1;
            xStart = x2;
        }
        else
        {
            yStart = y1;
            yEnd = y2;
            xStart = x1;
        }
        for (int y = yStart; y <= yEnd; ++y)
        {
            int x = xStart + (y - yStart) / slope;
            if (y < 0 || x < 0)
            {
                continue;
            }
            if (y >= height || x >= width)
            {
                continue;
            }
            DrawPixel(x, y, data, width, r, g, b);
        }
    }
}

void TextureDrawing::DrawRectangle(int x1, int y1, int x2, int y2, unsigned char* data, int width, int height,
    unsigned char r, unsigned char g, unsigned char b)
{
    for (int x = x1; x <= x2; ++x)
    {
        
        if (x >= width)
        {
            break;
        }
        if (x < 0)
        {
            continue;
        }
        for (int y = y1; y <= y2; ++y)
        {
            if(y >= height)
                break;
            if (y < 0)
            {
                continue;
            }
            DrawPixel(x, y, data, width, r, g, b);
        }
    }
}

void TextureDrawing::DrawCircle(int x, int y, int radius, unsigned char* data, int width, int height, unsigned char r,
    unsigned char g, unsigned char b)
{
    for(int i = -radius; i <= radius; ++i)
    {
        if(i>= width - x)
            break;
        if (i < -x)
        {
            continue;
        }
        for (int j = -radius; j <= radius; ++j)
        {
            if(j >= height - y)
                break;
            if (j < -y)
            {
                continue;
            }
            if (i * i + j * j <= radius * radius)
            {
                DrawPixel(x + i, y + j, data, width, r, g, b);
            }
        }
    }
}
