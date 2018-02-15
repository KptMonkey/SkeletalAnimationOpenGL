#pragma once
#include <GL/glew.h>
#include "Texture.hpp"

class FrameBuffer {

public:
    GLuint
    getFrameBuffer();
    void
    createDepthFrameBuffer(Texture & depthTexture);
    void
    bind();
    void
    unbind();
private:
    GLuint m_FrameBuffer;
};
