#include "GLTexture.h"

#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec2.hpp>




GLTexture::GLTexture()
{
    CreateBlankTexture(width,height,format);
}

GLTexture::~GLTexture()
{
    delete[] data;
}

void GLTexture::CreateBlankTexture(unsigned inWidth, unsigned inHeight, GLenum inFormat)
{
    if(inFormat != GL_RGB && inFormat != GL_RGBA)
    {
        std::cout << "Texture format not supported" << std::endl;
        return;
    }

    int dataSize = inWidth * inHeight * (inFormat == GL_RGB ? 3 : 4);

    delete[] data;
    data = new unsigned char[dataSize];

    for (int i = 0; i < dataSize; i++)
    {
        data[i] = 255;
    }
    format = inFormat;
    width = inWidth;
    height = inHeight;
}

void GLTexture::LoadFromFile(const char* path)
{
    GenerateOpenGlTexture();
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int nrChannels;
    data = stbi_load(path, &width, &height, &nrChannels, 0);
    format = nrChannels == 3 ? GL_RGB : GL_RGBA;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        CreateBlankTexture(16,16,GL_RGB);
    }
}

void GLTexture::GenerateOpenGlTexture()
{
    if(textureID != 0)
    {
        glDeleteTextures(1, &textureID);
    }
    glGenTextures(1, &textureID);
}

void GLTexture::SendDataToOpenGl() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    //To change if we want to load from texture file
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

unsigned int GLTexture::GetTextureId() const
{
    return textureID; 
}

glm::vec2 GLTexture::GetSize() const
{
    return glm::vec2(width, height);
}

void GLTexture::Fill(unsigned char r, unsigned char g, unsigned char b) const
{
    for (int row = 0; row < width; ++row)
    {
        for (int col = 0; col < height; ++col)
        {
            data[(row + col * width) * 3] = 0x00;
            data[(row + col * width) * 3 + 1] = 0x00;
            data[(row + col * width) * 3 + 2] = 0x00;
        }
    }
}

void GLTexture::SetTextureData(int inWidth, int inHeight, unsigned char* inData)
{
    width = inWidth;
    height = inHeight;
    data = inData;
}

unsigned char* GLTexture::GetTextureData()
{
    return data;
}
