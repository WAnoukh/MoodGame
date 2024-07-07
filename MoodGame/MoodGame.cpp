#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb_image.h>

#include "GameApp.h"

int main(int argc, char* argv[])
{
    GameApp& ga = GameApp::GetInstance();
    if (argc == 2)
    {
        std::string arg = argv[1];
        ga.Init(argv[1]);
    }else
    {
        ga.Init();
    }
    return ga.Run();
}
