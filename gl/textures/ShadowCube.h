#ifndef SHADOWCUBE_H
#define SHADOWCUBE_H

#include "gl/textures/TextureCube.h"

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

    TextureCube m_depthCube;

    void setupDepthCube();
};

}}

#endif // SHADOWCUBE_H
