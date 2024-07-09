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
    std::set<std::tuple<int,int>> registeredRoom;
    std::set<int> registeredCorner;
    for(int roomIndex = 0; roomIndex < world.rooms.size(); ++roomIndex)
    {
        EditableWorld::Room& room = world.rooms[roomIndex];
        
        RoomFeature roomFeature(roomIndex);
        features.push_back(std::make_shared<RoomFeature>(roomIndex));
        
        for (int RoomCornerIndex = 0; RoomCornerIndex < room.cornersIndexes.size(); ++RoomCornerIndex)
        {
            int cornerIndex = room.cornersIndexes[RoomCornerIndex];
            if(!registeredCorner.contains(cornerIndex))
            {
                registeredCorner.insert(cornerIndex);
                CornerFeature cornerFeature(cornerIndex);
                features.push_back(std::make_shared<CornerFeature>(cornerFeature));
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
                features.push_back(std::make_shared<WallFeature>(wallFeature));
            }
        }
    }
    PlayerFeature playerFeature;
    features.push_back(std::make_shared<PlayerFeature>(playerFeature));
    SortFeatures();
}

void FeatureManager::Draw(WorldEditorRenderer& worldEditorRenderer)
{
    for (int featureIndex : featuresIndexesSortedByDrawOrder)
    {
        features[featureIndex]->Draw(worldEditorRenderer);
    }
}

MapFeature* FeatureManager::FindSelectedFeature(WorldEditorRenderer& worldEditorRenderer, float x, float y)
{
    for (int featureIndex : featuresIndexesSortedBySelectOrder)
    {
        if (features[featureIndex]->CanSelect(worldEditorRenderer, x, y))
        {
            return &*features[featureIndex];
        }
    }
    return nullptr;
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
