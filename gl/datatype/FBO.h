#ifndef FBO_H
#define FBO_H

#include <memory>
#include <vector>

#include "gl/textures/TextureParameters.h"

namespace CS123 { namespace GL {

class RenderBuffer;
class Texture2D;

class FBO {
public:

    enum class DEPTH_STENCIL_ATTACHMENT { DEPTH_ONLY, DEPTH_ONLY_TEXTURE ,DEPTH_STENCIL, NONE };

    FBO(int numberOfColorAttachments, DEPTH_STENCIL_ATTACHMENT attachmentType, int m_width, int m_height,
        TextureParameters::WRAP_METHOD wrapMethod = TextureParameters::WRAP_METHOD::REPEAT,
        TextureParameters::FILTER_METHOD filterMethod = TextureParameters::FILTER_METHOD::LINEAR,
        GLenum type = GL_UNSIGNED_BYTE);
    ~FBO();

    void bind();
    void unbind();

    const Texture2D& getColorAttachment(int i) const;
    const RenderBuffer& getDepthStencilAttachment() const;
    const Texture2D& getDepthTextureAttachment() const;
    size_t getNumColorAttachments() const;

private:
    void generateColorAttachments(int count, TextureParameters::WRAP_METHOD wrapMethod,
                                  TextureParameters::FILTER_METHOD filterMethod, GLenum type);
    void generateColorAttachment(int i, TextureParameters::WRAP_METHOD wrapMethod,
                                 TextureParameters::FILTER_METHOD filterMethod, GLenum type);
    void generateDepthStencilAttachment();

    std::vector<Texture2D> m_colorAttachments;
    std::unique_ptr<Texture2D> m_depthTectureAttachment;
    std::unique_ptr<RenderBuffer> m_depthAttachment;
    DEPTH_STENCIL_ATTACHMENT m_depthStencilAttachmentType;

    unsigned int m_handle;
    int m_width;
    int m_height;
};

}}

#endif // FBO_H
