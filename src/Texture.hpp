#pragma once
#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture(){}
    ~Texture(){}
    void
    activate(int textureUnit);
    void
    load2DTexture(std::string path);
    void
    load2DTextureAlpha(std::string path);

    void
    createDepthTexture(int width, int height);
    GLuint
    getTexture();

private:
    GLuint m_Texture;
};
