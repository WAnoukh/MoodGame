#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/detail/type_vec.hpp>




class GLTexture
{
public:
    GLTexture();
    ~GLTexture();
    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;
    
    void CreateBlankTexture(unsigned inWidth, unsigned inHeight, GLenum inFormat);


    void LoadFromFile(const char* path);

    void GenerateOpenGlTexture();
    void SendDataToOpenGl() const;
    unsigned int GetTextureId() const;

    glm::vec2 GetSize() const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    void SetTextureData(int inWidth, int inHeight, unsigned char* inData);

    unsigned char* GetTextureData();
    
private:
    unsigned char* data = nullptr;
    int width = 32;
    int height = 32;
    unsigned int textureID = 0;
    GLenum format = GL_RGB;
};
