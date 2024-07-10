#pragma once
#include <list>
#include <memory>
#include <memory>
#include <vector>

#include "MapFeature.h"
#include "Features/CornerFeature.h"
#include "Features/WallFeature.h"

struct EditableWorld;

class FeatureManager
{
public:
    void InitializeFromWorld(EditableWorld& world);
    std::vector<std::shared_ptr<MapFeature>> ExctractFeatures(EditableWorld& world);
    void Draw(WorldEditorRenderer& worldEditorRenderer);
    std::shared_ptr<MapFeature> FindSelectedFeature(EditableWorld& editableWorld, float x, float y);

    int Extrude(EditableWorld& world, std::shared_ptr<WallFeature> featureToExtrude, std::shared_ptr<WallFeature>& extrudedFeature, bool
                separateRooms);
    std::shared_ptr<CornerFeature> AddCorner(EditableWorld& world, std::shared_ptr<WallFeature> targetWall, float x,
                                             float y);
private:
    std::vector<std::shared_ptr<MapFeature>> features;
    std::list<int> featuresIndexesSortedByDrawOrder;
    std::list<int> featuresIndexesSortedBySelectOrder;
    
    void SortFeatures();
};
