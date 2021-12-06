#include "TextureCube.h"

using namespace CS123::GL;

TextureCube::TextureCube(unsigned char *data, int size, GLenum internalFormat, GLenum format, GLenum type) :
    Texture()
{
    TextureCube::bind();

    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, size, size, 0, format, type, data);
    }

    TextureCube::unbind();
}

void TextureCube::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
}

void TextureCube::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
