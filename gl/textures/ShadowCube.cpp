#include "ShadowCube.h"
#include "GL/glew.h"

#include "gl/GLDebug.h"
#include "gl/textures/TextureCube.h"
#include "gl/textures/TextureParametersBuilder.h"

using namespace CS123::GL;

ShadowCube::ShadowCube(int size) :
    m_size(size),
    m_depthCube(TextureCube(nullptr, size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT))
{
    // Generate a new framebuffer using m_handle
    glGenFramebuffers(1, &m_handle);

    // Call bind()
    bind();

    // attach the depth texture tp FBO's depth buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCube.id(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // This will make sure your framebuffer was generated correctly!
    checkFramebufferStatus();

    // unbind the frame buffer
    unbind();
}

void ShadowCube::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    // Resize the viewport to our FBO's size
    glViewport(0, 0, m_size, m_size);
}

void ShadowCube::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const TextureCube& ShadowCube::getDepthCube() const {
    return m_depthCube;
}
