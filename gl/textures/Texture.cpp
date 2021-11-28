#include "Texture.h"

#include <cassert>
#include <utility>
#include <iostream>
#include <GL/glew.h>
#include "gl/GLDebug.h"

namespace CS123 { namespace GL {

Texture::Texture() :
    m_handle(0)
{
    // generate the texture
    glGenTextures(1, &m_handle);
}

Texture::Texture(Texture &&that) :
    m_handle(that.m_handle)
{
    that.m_handle = 0;
}

Texture& Texture::operator=(Texture &&that) {
    this->~Texture();
    m_handle = that.m_handle;
    that.m_handle = 0;
    return *this;
}

Texture::~Texture()
{
    std::cout << "TEXTURE DESTRUCTOR" << std::endl;
    glDeleteTextures(1, &m_handle);
}

unsigned int Texture::id() const {
    return m_handle;
}

}}
