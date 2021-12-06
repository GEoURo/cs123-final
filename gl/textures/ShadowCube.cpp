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

    // setup the depth cube correctly
    setupDepthCube();

    // This will make sure your framebuffer was generated correctly!
    checkFramebufferStatus();

    // unbind the frame buffer
    unbind();
}

void ShadowCube::setupDepthCube() {
    // setup the cube map parameters with direct openGL functions
    m_depthCube.bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    m_depthCube.unbind();

    // attach the depth texture tp FBO's depth buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCube.id(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
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
