#pragma once
#include <list>
#include <memory>
#include <vector>

#include "MapFeature.h"

struct EditableWorld;

class FeatureManager
{
public:

    void InitializeFromWorld(EditableWorld& world);
    void Draw(WorldEditorRenderer& worldEditorRenderer);
    MapFeature* FindSelectedFeature(WorldEditorRenderer& worldEditorRenderer, float x, float y);
private:
    std::vector<std::shared_ptr<MapFeature>> features;
    std::list<int> featuresIndexesSortedByDrawOrder;
    std::list<int> featuresIndexesSortedBySelectOrder;
    EditableWorld* world;
    
    void SortFeatures();
};
