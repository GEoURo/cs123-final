#ifndef SHADOWCUBE_H
#define SHADOWCUBE_H

#include "gl/textures/TextureCube.h"

#include <memory>

namespace CS123 { namespace GL {

class ShadowCube
{
public:
    ShadowCube(int size);

    void bind();
    void unbind();

    const TextureCube& getDepthCube() const;

private:
    int m_size;
    unsigned int m_handle;
    std::unique_ptr<TextureCube> m_depthCube;
};

}}

#endif // SHADOWCUBE_H
