#include "FBO.h"

#include "GL/glew.h"

#include "gl/GLDebug.h"
#include "gl/textures/RenderBuffer.h"
#include "gl/textures/DepthBuffer.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/TextureParametersBuilder.h"

using namespace CS123::GL;

FBO::FBO(int numberOfColorAttachments, DEPTH_STENCIL_ATTACHMENT attachmentType, int width, int height,
         TextureParameters::WRAP_METHOD wrapMethod,
         TextureParameters::FILTER_METHOD filterMethod, GLenum type) :
    m_depthStencilAttachmentType(attachmentType),
    m_handle(0),
    m_width(width),
    m_height(height)
{
    // Generate a new framebuffer using m_handle
    glGenFramebuffers(1, &m_handle);

    // Call bind() and fill it in with glBindFramebuffer
    bind();

    // Call generateColorAttachments() and fill in generateColorAttachment()
    generateColorAttachments(numberOfColorAttachments, wrapMethod, filterMethod, type);

    // Call generateDepthStencilAttachment()
    generateDepthStencilAttachment();

    // This will make sure your framebuffer was generated correctly!
    checkFramebufferStatus();

    // Call unbind()
    unbind();
}

FBO::~FBO()
{
    glDeleteFramebuffers(1, &m_handle);
}

void FBO::generateColorAttachments(int count, TextureParameters::WRAP_METHOD wrapMethod,
                                   TextureParameters::FILTER_METHOD filterMethod, GLenum type) {
    std::vector<GLenum> buffers;
    for (int i = 0; i < count; i++) {
        generateColorAttachment(i, wrapMethod, filterMethod, type);
        buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    // Call glDrawBuffers
    glDrawBuffers(count, &buffers[0]);
}

void FBO::generateDepthStencilAttachment() {
    switch(m_depthStencilAttachmentType) {
        case DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY:
        {
            m_depthAttachment = std::make_unique<DepthBuffer>(m_width, m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment->id());
            break;
        }
        case DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY_TEXTURE:
        {
            m_depthTectureAttachment = std::make_unique<Texture2D>(nullptr, m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
            TextureParametersBuilder builder;

            builder.setFilter(TextureParameters::FILTER_METHOD::NEAREST);
            builder.setWrap(TextureParameters::WRAP_METHOD::CLAMP_TO_BORDER);

            TextureParameters param = builder.build();
            param.applyTo(*m_depthTectureAttachment);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTectureAttachment->id(), 0);

            break;
        }
        case DEPTH_STENCIL_ATTACHMENT::DEPTH_STENCIL:
            // Left as an exercise to students
            break;
        case DEPTH_STENCIL_ATTACHMENT::NONE:
            break;
    }
}

void FBO::generateColorAttachment(int i, TextureParameters::WRAP_METHOD wrapMethod,
                                  TextureParameters::FILTER_METHOD filterMethod, GLenum type) {
    Texture2D tex(nullptr, m_width, m_height, type);
    TextureParametersBuilder builder;

    // - Set the filter method using builder.setFilter() with filterMethod
    // - Set the wrap method using builder.setWrap() with wrapMethod
    builder.setFilter(filterMethod);
    builder.setWrap(wrapMethod);

    TextureParameters parameters = builder.build();
    parameters.applyTo(tex);

    // Call glFramebufferTexture2D using tex.id()
    // this will attach the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex.id(), 0);

    m_colorAttachments.push_back(std::move(tex));
}

void FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    // Resize the viewport to our FBO's size
    glViewport(0, 0, m_width, m_height);
}

void FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Texture2D& FBO::getColorAttachment(int i) const {
    return m_colorAttachments.at(i);
}

const RenderBuffer& FBO::getDepthStencilAttachment() const {
    return *m_depthAttachment.get();
}

const Texture2D& FBO::getDepthTextureAttachment() const {
    return *m_depthTectureAttachment;
}
