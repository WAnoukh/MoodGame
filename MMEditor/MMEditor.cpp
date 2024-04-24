#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb_image.h>

#include "EditorApp.h"

int main(int argc, char* argv[])
{
    EditorApp& ga = EditorApp::GetInstance();
    ga.Init();
    return ga.Run();
}
