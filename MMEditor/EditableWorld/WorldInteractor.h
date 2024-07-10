#pragma once
#include "MapFeatures/FeatureManager.h"
#include "MapFeatures/MapFeature.h"

class WorldInteractor
{
public:
    explicit WorldInteractor(EditableWorld& editableWorld): world(&editableWorld){}

    void Tick(float DeltaTime);

    void MousePositionCallBackEvent(double newScreenMouseX, double newScreenMouseY, float newWorldMouseX, float newWorldMouseY);
    void MouseButtonCallBackEvent(int button, int action);
    void ScrollCallBackEvent(double xOffset, double yOffset);
    void KeyboardKeyCallBackEvent(int key, int scancode, int action, int mods);
    
    void RenderSelectedFeatureGUI();
    void InitializeFeatureManager();
    void DrawFeatures(WorldEditorRenderer& worldEditorRenderer);

    float GetZoom() const { return zoom; }
    void GetOffset(float& outX, float& outY) const { outX = xOffset; outY = yOffset; }

private:
    EditableWorld* world;
    FeatureManager featureManager;

    double lastScreenMouseX = 0.0f;
    double lastScreenMouseY = 0.0f;
    double screenMouseX = 0.0f;
    double screenMouseY = 0.0f;
    float screenMouseDx = 0.0f;
    float screenMouseDy = 0.0f;

    float lastWorldMouseX = 0.0f;
    float lastWorldMouseY = 0.0f;
    float worldMouseX = 0.0f;
    float worldMouseY = 0.0f;
    float worldMouseDx = 0.0f;
    float worldMouseDy = 0.0f;

    float zoom = 10.0f;
    float xOffset = 0;
    float yOffset = 0;

    bool panning = false;

    std::shared_ptr<MapFeature> hoveredFeature = nullptr;
    std::shared_ptr<MapFeature> selectedFeature = nullptr;
    bool dragging = false;
    
    void SetHoveredFeature(std::shared_ptr<MapFeature> feature);
    void SetSelectedFeature(std::shared_ptr<MapFeature> feature);
};
