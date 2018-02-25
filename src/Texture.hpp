#pragma once
#include <string>
#include <GL/glew.h>
enum class ColorChanel {
   RGB, RGBA
};

class Texture {
public:
    Texture(){}
    ~Texture(){}

    void
    activate(int textureUnit);

    void
    load2DTexture(std::string path, ColorChanel c);

    void
    createDepthTexture(int width, int height);

    GLuint
    getTexture();

private:
    GLuint m_Texture;
};
