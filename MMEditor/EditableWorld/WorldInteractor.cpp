#include "WorldInteractor.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "../../WindowMaker/WindowInput.h"

#include "MapFeatures/Features/WallFeature.h"

void WorldInteractor::Tick(float DeltaTime)
{
    worldMouseDx = worldMouseX - lastWorldMouseX;
    lastWorldMouseX = worldMouseX;
    worldMouseDy = worldMouseY - lastWorldMouseY;
    lastWorldMouseY = worldMouseY;
    screenMouseDx = screenMouseX - lastScreenMouseX;
    lastScreenMouseX = screenMouseX;
    screenMouseDy = screenMouseY - lastScreenMouseY;
    lastScreenMouseY = screenMouseY;
    
    // Update editor panning
    if (panning)
    {
        xOffset -= screenMouseDx;
        yOffset += screenMouseDy;
    }

    if(!panning)
    {
        std::shared_ptr<MapFeature> foundFeature = featureManager.FindSelectedFeature(*world, worldMouseX, worldMouseY);
        SetHoveredFeature(foundFeature);
    }else
    {
        SetHoveredFeature(nullptr);
    }

    // Move selected Feature
    if(selectedFeature != nullptr && dragging)
    {
        selectedFeature->Drag(*world, worldMouseDx, worldMouseDy);
    }
}

void WorldInteractor::MousePositionCallBackEvent(double newScreenMouseX, double newScreenMouseY, float newWorldMouseX,
                                                 float newWorldMouseY)
{
    screenMouseX = newScreenMouseX;
    screenMouseY = newScreenMouseY;
    worldMouseX = newWorldMouseX;
    worldMouseY = newWorldMouseY;
}

void WorldInteractor::MouseButtonCallBackEvent(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            panning = true;
        }
        else if (action == GLFW_RELEASE)
        {
            panning = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (WindowInput::IsShiftPressed() && action == GLFW_PRESS &&
            (hoveredFeature != nullptr || selectedFeature != nullptr))
        {
            std::shared_ptr<WallFeature> wallFeature;
            if (hoveredFeature)
            {
                wallFeature = std::dynamic_pointer_cast<WallFeature>(hoveredFeature);
            }
            else
            {
                wallFeature = std::dynamic_pointer_cast<WallFeature>(selectedFeature);
            }
            if (wallFeature)
            {
                auto newCorner = featureManager.AddCorner(*world, wallFeature, worldMouseX, worldMouseY);
                SetSelectedFeature(newCorner);
                dragging = true;
            }
            return;
        }
        if (action == GLFW_PRESS)
        {
            std::shared_ptr<MapFeature> foundFeature = featureManager.FindSelectedFeature(*world, worldMouseX, worldMouseY);
            if(foundFeature!=nullptr)
            {
                dragging = true;
                std::cout << "Pressed" << std::endl;
            }
            SetSelectedFeature(foundFeature);
        }
        else if (action == GLFW_RELEASE)
        {
            std::cout << "Released" << std::endl;
            dragging = false;
        }
    }
}

void WorldInteractor::ScrollCallBackEvent(double xScroll, double ySroll)
{
    float zoomRate = (1 - ySroll * 0.1f);
    zoom *= zoomRate;
    xOffset /= zoomRate;
    yOffset /= zoomRate;
}

void WorldInteractor::KeyboardKeyCallBackEvent(int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        if(key == GLFW_KEY_E)
        {
            // Extrusion
            if (selectedFeature != nullptr)
            {
                std::shared_ptr<WallFeature> wallFeature = std::dynamic_pointer_cast<WallFeature>(selectedFeature);
                if (wallFeature)
                {
                    bool separateRooms = WindowInput::IsShiftPressed();
                    std::shared_ptr<WallFeature> extrudedFeature;
                    int result = featureManager.Extrude(*world, wallFeature, extrudedFeature, separateRooms);
                    if (result >= 0)
                    {
                        SetSelectedFeature(extrudedFeature);
                        dragging = true;
                    }
                    else
                    {
                        std::cout << "Extrusion failed: " << result << std::endl;
                    }
                }
                else
                {
                    std::cout << "Extrusion failed: Selected feature is not a wall" << std::endl;
                }
            }else
            {
                std::cout << "Extrusion failed: No feature selected" << std::endl;
            }
        }
    }
}

void WorldInteractor::RenderSelectedFeatureGUI()
{
    if (selectedFeature != nullptr)
    {
        selectedFeature->RenderGui(*world);
    }
}

void WorldInteractor::InitializeFeatureManager()
{
    selectedFeature = nullptr;
    hoveredFeature = nullptr;
    featureManager.InitializeFromWorld(*world);
}

void WorldInteractor::DrawFeatures(WorldEditorRenderer& worldEditorRenderer)
{
    featureManager.Draw(worldEditorRenderer);
}

void WorldInteractor::SetHoveredFeature(std::shared_ptr<MapFeature> feature)
{
    if (hoveredFeature != nullptr)
        hoveredFeature->SetState(MapFeature::Normal);
    if(feature != nullptr && feature == selectedFeature)
        return;
    hoveredFeature = feature;
    if (hoveredFeature != nullptr)
        hoveredFeature->SetState(MapFeature::Hovered);
}

void WorldInteractor::SetSelectedFeature(std::shared_ptr<MapFeature> feature)
{
    if (selectedFeature != nullptr)
        selectedFeature->SetState(MapFeature::Normal);
    if(feature != nullptr && feature == hoveredFeature)
        hoveredFeature = nullptr;
    selectedFeature = feature;
    if (selectedFeature != nullptr)
        selectedFeature->SetState(MapFeature::Selected);
}
