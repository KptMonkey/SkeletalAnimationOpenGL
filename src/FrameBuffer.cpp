#include "FrameBuffer.hpp"


GLuint
FrameBuffer::getFrameBuffer() {
    return m_FrameBuffer;
}

void
FrameBuffer::createDepthFrameBuffer(Texture & depthTexture) {
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.getTexture(),0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

}

void
FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void
FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
