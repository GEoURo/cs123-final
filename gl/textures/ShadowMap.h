#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "gl/textures/Texture2D.h"

namespace CS123 { namespace GL {

class ShadowMap {
public:
    ShadowMap(int width, int height);

    void bind();
    void unbind();

    const Texture2D& getDepthMap() const;

private:
    int m_width;
    int m_height;
    unsigned int m_handle;

    Texture2D m_depthMap;

    void setupDepthMap();
};

}}
#endif // SHADOWMAP_H
