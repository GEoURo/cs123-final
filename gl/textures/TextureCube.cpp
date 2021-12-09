#include "TextureCube.h"

using namespace CS123::GL;

TextureCube::TextureCube(void *data, int size, GLenum internalFormat, GLenum format, GLenum type) :
    Texture()
{
    TextureCube::bind();

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, size, size, 0, format, type, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, size, size, 0, format, type, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, size, size, 0, format, type, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, size, size, 0, format, type, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, size, size, 0, format, type, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, size, size, 0, format, type, data);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    TextureCube::unbind();
}

void TextureCube::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
}

void TextureCube::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
