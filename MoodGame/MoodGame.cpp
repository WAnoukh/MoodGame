#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb_image.h>

#include "GameApp.h"

int main(int argc, char* argv[])
{
    GameApp& ga = GameApp::GetInstance();
    ga.Init();
    return ga.Run();
}
