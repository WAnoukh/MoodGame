#include "ByteTexture.h"
#include <iostream>

#include "../stb_image.h"

ByteTexture* ByteTexture::LoadTexture(char const* path)
{
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        return new ByteTexture{data, width, height, nrComponents};
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return nullptr;
    }
}
