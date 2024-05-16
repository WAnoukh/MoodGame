#pragma once
#include "EditableWorld.h"
#include "../../src/World/WorldReader.h"

class World;

namespace WorldConverter
{
    void ConvertWorld(EditableWorld& outEditableWorld, World& world);
    void ConvertEditableWorld(World& outWorld, EditableWorld& editableWorld);
};