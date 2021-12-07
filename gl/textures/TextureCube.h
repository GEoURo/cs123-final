#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "gl/textures/Texture.h"
#include "GL/glew.h"

namespace CS123 { namespace GL {

class TextureCube : public Texture
{
public:
    TextureCube(void *data, int size, GLenum internalFormat, GLenum format, GLenum type);

    virtual void bind() const override;
    virtual void unbind() const override;
};

}}

#endif // TEXTURECUBE_H
