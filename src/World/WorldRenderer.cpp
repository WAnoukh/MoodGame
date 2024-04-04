#include "WorldRenderer.h"

#include <cmath>
#include <corecrt_math_defines.h>
#include <iostream>
#include <list>
#include <queue>
#include <set>

#include "World.h"

#define LERP(a,b,c) a + (b-a) * c
#define CLAMP(minVal, maxVal, val) std::min(std::max(minVal, val), maxVal)
#define FAR 1000.0f
#define NEAR 0.0f

bool linesIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x,
                       float& y)
{
    float deno = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (abs(deno) <= 0.000001f)
    {
        x = NAN;
        y = NAN;
        return false;
    }
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / deno;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / deno;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        x = x1 + t * (x2 - x1);
        y = y1 + t * (y2 - y1);
        return true;
    }
    return false;
}

struct Col
{
    Col(char r, char g, char b): r(r), g(g), b(b)
    {
    };
    char r, g, b;
};

std::vector<Col> sectorColor = {
    Col(0xff, 0x00, 0x00), Col(0x00, 0xff, 0x00), Col(0x00, 0x00, 0xff), Col(0xff, 0xff, 0x00), Col(0xff, 0x00, 0xff),
    Col(0x00, 0xff, 0xff), Col(0xff, 0xff, 0xff), Col(0x00, 0x00, 0x00)
};

void WorldRenderer::Render(const size_t width, const size_t height, unsigned char* outData)
{
    const float ratio = static_cast<float>(width) / static_cast<float>(height);
    for (size_t row = 0; row < width; ++row)
    {
        for (size_t col = 0; col < height; ++col)
        {
            outData[(row + col * width) * 3] = 0x00;
            outData[(row + col * width) * 3 + 1] = 0x00;
            outData[(row + col * width) * 3 + 2] = 0x00;
        }
    }

    struct ColumnDrawData
    {
        size_t firstRow;
        size_t lastRow;
    };

    auto* columnData = new ColumnDrawData[width];
    for (size_t i = 0; i < width; ++i)
    {
        columnData[i] = {0, height};
    }

    size_t curentSector = world->GetSectorAtPos(camX, camY);
    if (curentSector == 0)
    {
        return;
    }
    struct QueueElement
    {
        size_t sectorIndex;
        size_t colStart;
        size_t colEnd;
    };

    std::queue<QueueElement> queue;
    queue.push({curentSector, static_cast<size_t>(0), width});
    camZ = world->GetSector(curentSector)->floor + 0.9f;

    const float
        minAngle = (-HFOV) * (M_PI_2 / 180),
        maxAngle = (+HFOV) * (M_PI_2 / 180);
    const float fflx = FAR * sin(minAngle);
    const float ffly = FAR * cos(minAngle);
    const float ffrx = FAR * sin(maxAngle);
    const float ffry = FAR * cos(maxAngle);
    const float fnlx = NEAR * sin(minAngle);
    const float fnly = NEAR * cos(minAngle);
    const float fnrx = NEAR * sin(maxAngle);
    const float fnry = NEAR * cos(maxAngle);
    const float screenDist = ratio / (2.0f * tan(HFOV * M_PI_2 / 180));
    //const float screenDistY = height / 2.0f* tan(VFOV * M_PI_2 / 180);
    //const float screenDistY2 = 1.0f / tan(HFOV * M_PI_2 / 180) ;
    //const float screenDistY3 = 1.0f / tan(VFOV  * M_PI_2 / 180) * ratio;
    while (!queue.empty())
    {
        QueueElement element = queue.front();
        Sector sector = *world->GetSector(element.sectorIndex);
        size_t minCol = element.colStart;
        size_t maxCol = element.colEnd;
        queue.pop();
        for (size_t i = sector.firstWall; i < sector.firstWall + sector.numWalls; i++)
        {
            Wall* wall = world->GetWall(i);
            //center wall on camera and reorient it
            const float tempOrientedX1 = wall->x1 - camX;
            const float tempOrientedY1 = wall->y1 - camY;
            const float tempOrientedX2 = wall->x2 - camX;
            const float tempOrientedY2 = wall->y2 - camY;
            float x1 = tempOrientedX1 * cos(-camAngle) - tempOrientedY1 * sin(-camAngle);
            float y1 = tempOrientedX1 * sin(-camAngle) + tempOrientedY1 * cos(-camAngle);
            float x2 = tempOrientedX2 * cos(-camAngle) - tempOrientedY2 * sin(-camAngle);
            float y2 = tempOrientedX2 * sin(-camAngle) + tempOrientedY2 * cos(-camAngle);

            if (y1 <= 0 && y2 <= 0)
            {
                //wall is behind the camera
                continue;
            }
            float startOffset = 0;
            float endOffset = 0;
            float angle1 = atan2(x1, y1);
            float angle2 = atan2(x2, y2);
            float tmpX, tmpY;
            if (linesIntersection(x1, y1, x2, y2, fflx, ffly, fnlx, fnly, tmpX, tmpY))
            {
                startOffset = sqrt((tmpX - x1) * (tmpX - x1) + (tmpY - y1) * (tmpY - y1));
                x1 = tmpX;
                y1 = tmpY;
                angle1 = minAngle;
            }
            else
            {
                if (angle1 < minAngle)
                {
                    continue;
                }
            }
            if (linesIntersection(x1, y1, x2, y2, ffrx, ffry, fnrx, fnry, tmpX, tmpY))
            {
                endOffset = sqrt((tmpX - x2) * (tmpX - x2) + (tmpY - y2) * (tmpY - y2));
                x2 = tmpX;
                y2 = tmpY;
                angle2 = maxAngle;
            }
            else
            {
                if (angle2 > maxAngle)
                {
                    continue;
                }
            }

            if (angle1 > angle2)
            {
                continue;
            }
            const float dist1 = sqrt(x1 * x1 + y1 * y1);
            const float dist2 = sqrt(x2 * x2 + y2 * y2);

            //const float firstColumnf =
            float temps1 = (1 - tan(angle1) / tan(minAngle)) * width / 2;
            const float virtualStartf = std::max(0.0f,
                                                 round(temps1)
            );
            float temps2 = (1 + tan(angle2) / tan(maxAngle)) * width / 2;
            const float virtualEndf = std::min(static_cast<float>(width),
                                               round(temps2)
            );


            const size_t virtualFirstCol = static_cast<size_t>(virtualStartf);
            const size_t virtualLastCol = static_cast<size_t>(virtualEndf);
            const size_t realFirstCol = std::max(minCol, virtualFirstCol);
            const size_t realLastCol = std::min(maxCol, virtualLastCol);

            if (virtualLastCol < virtualFirstCol)
            {
                continue;
            }

            if (wall->portal)
            {
                if (realFirstCol < realLastCol)
                {
                    queue.push({wall->portal, realFirstCol, realLastCol});
                }
            }

            //compute perpendiculare distance
            //const float wall1FirstRow = screenDist * (camZ- sector.ceil) / y1 + height / 2;
            const float wall1FirstRow = (screenDist * (camZ - sector.ceil) / y1 + 0.5f) * height;
            const float wall1LastRow = (screenDist * (camZ - sector.floor) / y1 + 0.5f) * height;
            const float wall2FirstRow = (screenDist * (camZ - sector.ceil) / y2 + 0.5f) * height;
            const float wall2LastRow = (screenDist * (camZ - sector.floor) / y2 + 0.5f) * height;

            float portal1FirstRow = -1;
            float portal2FirstRow = -1;
            float portal1LastRow = -1;
            float portal2LastRow = -1;
            bool bottomPortal = false;
            bool topPortal = false;
            if (wall->portal)
            {
                Sector portalSector = *world->GetSector(wall->portal);
                if (portalSector.ceil < sector.ceil)
                {
                    portal1FirstRow = (screenDist * (camZ - portalSector.ceil) / y1 + 0.5f) * height;
                    portal2FirstRow = (screenDist * (camZ - portalSector.ceil) / y2 + 0.5f) * height;
                    topPortal = true;
                }
                if (portalSector.floor > sector.floor)
                {
                    portal1LastRow = (screenDist * (camZ - portalSector.floor) / y1 + 0.5f) * height;
                    portal2LastRow = (screenDist * (camZ - portalSector.floor) / y2 + 0.5f) * height;
                    bottomPortal = true;
                }
            }
            for (size_t col = realFirstCol; col < realLastCol; ++col)
            {
                const float drawRate = static_cast<float>(col - virtualFirstCol) / static_cast<float>(virtualLastCol -
                    virtualFirstCol);
                const int wallVirtualFirstRow = LERP(wall1FirstRow, wall2FirstRow, drawRate);
                const int wallVirtualLastRow = LERP(wall1LastRow, wall2LastRow, drawRate);
                //int newFirstRow = std::max(static_cast<int>(columnData[col].firstRow), wallFirstRow);
                int newRealFirstRow = CLAMP(static_cast<int>(columnData[col].firstRow),
                                            static_cast<int>(columnData[col].lastRow), wallVirtualFirstRow);
                //int newLastRow = std::min(static_cast<int>(columnData[col].lastRow), wallLastRow);
                int newRealLastRow = CLAMP(static_cast<int>(columnData[col].firstRow),
                                           static_cast<int>(columnData[col].lastRow), wallVirtualLastRow);
                {
                    const float rayY = screenDist;
                    const float rayX = (col / static_cast<float>(width) - 0.5f) * ratio;
                    float rayZ;
                    for (size_t row = newRealLastRow; row < columnData[col].lastRow; ++row)
                    {
                        float rotatedRayX = rayY * std::cos(camAngle) + rayX * std::sin(camAngle);
                        float rotatedRayY = rayY * std::sin(camAngle) - rayX * std::cos(camAngle);
                        rayZ = (row / static_cast<float>(height)) - 0.5f;
                        float floorX = (sector.floor - camZ) * rotatedRayX / (rayZ);
                        float floorY = (sector.floor - camZ) * rotatedRayY / (rayZ);
                        floorX -= camY;
                        floorY += camX;

                        int textureX = abs(static_cast<int>(floorX * ceilingTexture->width)) % ceilingTexture->width;
                        int textureY = abs(static_cast<int>(floorY * ceilingTexture->height)) % ceilingTexture->
                            height;

                        // Draw floor
                        outData[(col + row * width) * 3] = ceilingTexture->data[(textureX + textureY * ceilingTexture->
                            width) * 3];
                        outData[(col + row * width) * 3 + 1] = ceilingTexture->data[(textureX + textureY *
                            ceilingTexture->width) * 3 + 1];
                        outData[(col + row * width) * 3 + 2] = ceilingTexture->data[(textureX + textureY *
                            ceilingTexture->width) * 3 + 2];
                    }
                    for (size_t row = columnData[col].firstRow; row < newRealFirstRow; ++row)
                    {
                        float rotatedRayX = rayY * std::cos(camAngle) + rayX * std::sin(camAngle);
                        float rotatedRayY = rayY * std::sin(camAngle) - rayX * std::cos(camAngle);
                        rayZ = (row / static_cast<float>(height)) - 0.5f;
                        float floorX = (sector.ceil - camZ) * rotatedRayX / (rayZ);
                        float floorY = (sector.ceil - camZ) * rotatedRayY / (rayZ);
                        floorX -= camY;

                        float temp = std::fmod(abs(floorX), 1.0f);
                        if (temp < 0.1f)
                        {
                            outData[(col + row * width) * 3] = 0xff;
                            outData[(col + row * width) * 3 + 1] = 0xff;
                            outData[(col + row * width) * 3 + 2] = 0xff * 0;
                            continue;
                        }

                        //float rfloorX = floorX * std::cos(-camAngle) - floorY * std::sin(-camAngle);
                        //float rfloorY = floorX * std::sin(-camAngle) + floorY * std::cos(-camAngle);

                        float textureX = abs(static_cast<float>(floorX));
                        float textureY = abs(static_cast<float>(floorY));

                        // Draw floor
                        outData[(col + row * width) * 3] = 0xff * textureX;
                        outData[(col + row * width) * 3 + 1] = 0xff * 0;
                        outData[(col + row * width) * 3 + 2] = 0xff * 0;
                    }
                }
                if (!wall->portal)
                {
                    float rayY = screenDist;
                    float inter = col / static_cast<float>(width) - 0.5f;
                    float rayX = (inter) * ratio;
                    float angleTemp = atan2(rayX, rayY);
                    float angleTemp2 = minAngle;
                    float tmpX, tmpY;
                    if (linesIntersection(x1, y1, x2, y2, 0, 0, rayX * 1000.f, rayY * 1000.f, tmpX, tmpY))
                    {
                        tmpX -= x1;
                        tmpY -= y1;
                        if (newRealLastRow < newRealFirstRow)
                        {
                            continue;
                        }
                        for (size_t row = newRealFirstRow; row < newRealLastRow; ++row)
                        {
                            // Draw wall
                            int textureY = (row - wallVirtualFirstRow) / static_cast<float>(wallVirtualLastRow -
                                wallVirtualFirstRow) * wallTexture->height;
                            float partialLength = sqrt(tmpX * tmpX + tmpY * tmpY);
                            int textureX = abs(static_cast<int>((startOffset + partialLength) * wallTexture->width)) %
                                wallTexture->width;
                            outData[(col + row * width) * 3] = wallTexture->data[(textureX + textureY * wallTexture->
                                width) * 3];
                            outData[(col + row * width) * 3 + 1] = wallTexture->data[(textureX + textureY * wallTexture
                                ->width) * 3 + 1];
                            outData[(col + row * width) * 3 + 2] = wallTexture->data[(textureX + textureY * wallTexture
                                ->width) * 3 + 2];
                        }
                    }
                    columnData[col].firstRow = 1;
                    columnData[col].lastRow = 0;
                }
                else
                {
                    Sector* portalSector = world->GetSector(wall->portal);
                    if (topPortal)
                    {
                        //int portalFirstRow = std::max(0.0f,);
                        int portalFirstRow = CLAMP(0.0f, static_cast<float>(height),
                                                   LERP(portal1FirstRow, portal2FirstRow, drawRate));
                        float rayY = screenDist;
                        float rayX = (col / (static_cast<float>(width)) - 0.5f) * ratio;
                        float tmpX, tmpY;
                        if (linesIntersection(x1, y1, x2, y2, 0, 0, rayX * 1000.f, rayY * 1000.f, tmpX, tmpY))
                        {
                            tmpX -= x1;
                            tmpY -= y1;
                            for (size_t row = newRealFirstRow; row < portalFirstRow; ++row)
                            {
                                // Draw portal top wall
                                int textureY = (row - wallVirtualFirstRow) / static_cast<float>(wallVirtualLastRow -
                                    wallVirtualFirstRow) * wallTexture->height;
                                float partialLength = sqrt(tmpX * tmpX + tmpY * tmpY);
                                int textureX = abs(static_cast<int>((startOffset + partialLength) * wallTexture->width))
                                    % wallTexture->width;
                                outData[(col + row * width) * 3] = wallTexture->data[(textureX + textureY * wallTexture
                                    ->width) * 3];
                                outData[(col + row * width) * 3 + 1] = wallTexture->data[(textureX + textureY *
                                    wallTexture->width) * 3 + 1];
                                outData[(col + row * width) * 3 + 2] = wallTexture->data[(textureX + textureY *
                                    wallTexture->width) * 3 + 2];
                            }
                        }
                        columnData[col].firstRow = CLAMP(newRealFirstRow, static_cast<int>(height), portalFirstRow);
                    }
                    else
                    {
                        columnData[col].firstRow = CLAMP(0, static_cast<int>(height), newRealFirstRow);
                    }
                    if (bottomPortal)
                    {
                        // Draw portal bottom wall
                        //int portalLastRow = std::min(static_cast<float>(height), LERP(portal1LastRow, portal2LastRow, drawRate));
                        int portalLastRow = CLAMP(0.0f, static_cast<float>(height),
                                                  LERP(portal1LastRow, portal2LastRow, drawRate));
                        float rayY = screenDist;
                        float rayX = (col / (static_cast<float>(width)) - 0.5f) * ratio;
                        float tmpX, tmpY;
                        if (linesIntersection(x1, y1, x2, y2, 0, 0, rayX * 1000.f, rayY * 1000.f, tmpX, tmpY))
                        {
                            tmpX -= x1;
                            tmpY -= y1;
                            for (size_t row = portalLastRow; row < newRealLastRow; ++row)
                            {
                                int textureY = (row - wallVirtualFirstRow) / static_cast<float>(wallVirtualLastRow -
                                    wallVirtualFirstRow) * wallTexture->height;
                                float partialLength = sqrt(tmpX * tmpX + tmpY * tmpY);
                                int textureX = abs(static_cast<int>((startOffset + partialLength) * wallTexture->width))
                                    % wallTexture->width;
                                outData[(col + row * width) * 3] = wallTexture->data[(textureX + textureY * wallTexture
                                    ->width) * 3];
                                outData[(col + row * width) * 3 + 1] = wallTexture->data[(textureX + textureY *
                                    wallTexture->width) * 3 + 1];
                                outData[(col + row * width) * 3 + 2] = wallTexture->data[(textureX + textureY *
                                    wallTexture->width) * 3 + 2];
                            }
                        }
                        columnData[col].lastRow = CLAMP(0, newRealLastRow, portalLastRow);
                    }
                    else
                    {
                        columnData[col].lastRow = CLAMP(0, static_cast<int>(height), newRealLastRow);
                    }
                }
            }
        }
    }
    delete[] columnData;
}


