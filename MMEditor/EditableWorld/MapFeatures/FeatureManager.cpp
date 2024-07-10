#include "FeatureManager.h"

#include <iostream>
#include <ostream>
#include <set>


#include "../EditableWorld.h"
#include "Features/CornerFeature.h"
#include "Features/RoomFeature.h"
#include "Features/WallFeature.h"
#include "MapFeature.h"
#include "Features/PlayerFeature.h"

void FeatureManager::InitializeFromWorld(EditableWorld& world)
{
    features.clear();
    featuresIndexesSortedByDrawOrder.clear();
    featuresIndexesSortedBySelectOrder.clear();
    features = ExctractFeatures(world);
    SortFeatures();
}

std::vector<std::shared_ptr<MapFeature>> FeatureManager::ExctractFeatures(EditableWorld& world)
{
    std::vector<std::shared_ptr<MapFeature>> extractedFeatures;
    std::set<std::tuple<int,int>> registeredRoom;
    std::set<int> registeredCorner;
    for(int roomIndex = 0; roomIndex < world.rooms.size(); ++roomIndex)
    {
        EditableWorld::Room& room = world.rooms[roomIndex];
        
        RoomFeature roomFeature(roomIndex);
        extractedFeatures.push_back(std::make_shared<RoomFeature>(roomIndex));
        
        for (int RoomCornerIndex = 0; RoomCornerIndex < room.cornersIndexes.size(); ++RoomCornerIndex)
        {
            int cornerIndex = room.cornersIndexes[RoomCornerIndex];
            if(!registeredCorner.contains(cornerIndex))
            {
                registeredCorner.insert(cornerIndex);
                CornerFeature cornerFeature(cornerIndex);
                extractedFeatures.push_back(std::make_shared<CornerFeature>(cornerFeature));
            }
            int nexCornerIndex = room.cornersIndexes[(RoomCornerIndex + 1) % room.cornersIndexes.size()];
            if (cornerIndex > nexCornerIndex)
            {
                std::swap(cornerIndex, nexCornerIndex);
            }
            std::tuple<int,int> wall = std::make_tuple(cornerIndex, nexCornerIndex);
            if (!registeredRoom.contains(wall))
            {
                registeredRoom.insert(wall);
                WallFeature wallFeature(cornerIndex, nexCornerIndex);
                extractedFeatures.push_back(std::make_shared<WallFeature>(wallFeature));
            }
        }
    }
    PlayerFeature playerFeature;
    extractedFeatures.push_back(std::make_shared<PlayerFeature>(playerFeature));
    return extractedFeatures;
}

void FeatureManager::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    for (int featureIndex : featuresIndexesSortedByDrawOrder)
    {
        features[featureIndex]->Draw(worldEditorRenderer);
    }
}

std::shared_ptr<MapFeature> FeatureManager::FindSelectedFeature(EditableWorld& editableWorld, float x, float y)
{
    for (int featureIndex : featuresIndexesSortedBySelectOrder)
    {
        if (features[featureIndex]->CanSelect(editableWorld, x, y))
        {
            return features[featureIndex];
        }
    }
    return nullptr;
}

int FeatureManager::Extrude(EditableWorld& world, std::shared_ptr<WallFeature> featureToExtrude, std::shared_ptr<WallFeature>& extrudedFeature, bool
                            separateRooms)
{
    auto corner1 = featureToExtrude->GetCorner1();
    auto corner2 = featureToExtrude->GetCorner2();
    int extrudedCorner1, extrudedCorner2;
    int result = WorldEditor::Extrude(world, corner1, corner2, extrudedCorner1, extrudedCorner2, separateRooms);
    InitializeFromWorld(world);
    if (result < 0)
    {
        return result;
    }
    for (auto& feature : features)
    {
        std::shared_ptr<WallFeature> wallFeature = std::dynamic_pointer_cast<WallFeature>(feature);
        if (wallFeature)
        {
            if(extrudedCorner1 == wallFeature->GetCorner1() && extrudedCorner2 == wallFeature->GetCorner2())
            {
                extrudedFeature = wallFeature;
                break;
            }
        }
    }
    return 0;
}


void FeatureManager::SortFeatures()
{
    std::list<std::tuple<int,int>> featuresDrawOrder;
    std::list<std::tuple<int,int>> featuresSelectOrder;
    for (int i = 0; i < features.size(); ++i)
    {
        MapFeature& feature = *features[i];
        featuresDrawOrder.emplace_back(i, feature.GetDrawPriority());
        featuresSelectOrder.emplace_back(i, feature.GetSelectPriority());
    }
    
    featuresDrawOrder.sort([](std::tuple<int,int> a, std::tuple<int,int> b)
    {
        return std::get<1>(a) < std::get<1>(b);
    });
    featuresSelectOrder.sort([](std::tuple<int,int> a, std::tuple<int,int> b)
    {
        return std::get<1>(a) > std::get<1>(b);
    });
    
    for(auto& feature : featuresDrawOrder)
    {
        featuresIndexesSortedByDrawOrder.emplace_back(std::get<0>(feature));
    }
    for(auto& feature : featuresSelectOrder)
    {
        featuresIndexesSortedBySelectOrder.emplace_back(std::get<0>(feature));
    }
}
