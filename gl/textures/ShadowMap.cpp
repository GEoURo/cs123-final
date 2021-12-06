#include "ShadowMap.h"
#include "GL/glew.h"

#include "gl/GLDebug.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/TextureParametersBuilder.h"

using namespace CS123::GL;

ShadowMap::ShadowMap(int width, int height) :
    m_width(width),
    m_height(height),
    m_depthMap(Texture2D(nullptr, m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT))
{
    // Generate a new framebuffer using m_handle
    glGenFramebuffers(1, &m_handle);

    // Call bind()
    bind();

    // generate depth map
    setupDepthMap();

    // This will make sure your framebuffer was generated correctly!
    checkFramebufferStatus();

    // Call unbind()
    unbind();
}

void ShadowMap::setupDepthMap() {
    TextureParametersBuilder builder;

    builder.setFilter(TextureParameters::FILTER_METHOD::NEAREST);
    builder.setWrap(TextureParameters::WRAP_METHOD::REPEAT);

    TextureParameters param = builder.build();
    param.applyTo(m_depthMap);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap.id(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void ShadowMap::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    // Resize the viewport to our FBO's size
    glViewport(0, 0, m_width, m_height);
}

void ShadowMap::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Texture2D& ShadowMap::getDepthMap() const {
    return m_depthMap;
}

