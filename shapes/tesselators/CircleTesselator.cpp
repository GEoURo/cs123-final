#include "CircleTesselator.h"
#include <math.h>
#include <iostream>

using namespace std;
using namespace glm;

CircleTesselator::CircleTesselator(vec3 center, vec3 normal) :
    BaseTesselator(),
    m_center(center),
    m_normal(normal),
    m_stackNum(1),
    m_sliceNum(3)
{

}

void CircleTesselator::setStackNum(int stackNum) {
    if (stackNum == m_stackNum) {
        return;
    }

    m_stackNum = std::max(1, stackNum);
    m_cacheAvailable = false;
}

void CircleTesselator::setSliceNum(int sliceNum) {
    if (sliceNum == m_sliceNum) {
        return;
    }

    m_sliceNum = std::max(3, sliceNum);
    m_cacheAvailable = false;
}

void CircleTesselator::setCenter(glm::vec3 center) {
    if (center == m_center) {
        return;
    }

    m_center = center;
    m_cacheAvailable = false;
}

void CircleTesselator::setNormal(glm::vec3 normal) {
    if (normal == m_normal) {
        return;
    }

    m_normal = normal;
    m_cacheAvailable = false;
}

int CircleTesselator::verticesNum() {
    return m_sliceNum * (2 * m_stackNum - 1) * 3;;
}

void CircleTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        return;
    }

    std::cout << "Circle tesselate called" << std::endl;

    float radianStep = 2.f * M_PI / (float)m_sliceNum;
    float radiusStep = 0.5f / (float)m_stackNum;

    // we are assuming m_normal is parallel to y-axis
    if (m_normal.y == -1.f) {
        radianStep *= -1;
    }

    auto calPosition = [&radianStep, &radiusStep, this](float i, float j) -> vec3 {
        float r = j * radiusStep;
        vec3 ret = vec3(r * glm::cos(radianStep * i), 0.f, r * glm::sin(radianStep * i)) + m_center;
        return ret;
    };

    auto calTextureUV = [this](vec3 &pos) -> vec2 {
        float u = pos.x + 0.5f;
        float v = m_normal.y == 1 ? pos.z + 0.5 : 0.5 - pos.z;
        return vec2(u, v);
    };

    int idx = 0;
    for (int i = 0; i < m_sliceNum; i++) {
        for (int j = 0; j < m_stackNum; j++) {
            // The traverse is going in the clockwise direction, from center to edge
            m_vertices[idx] = calPosition(i, j);
            m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
            m_vNormals[idx++] = m_normal;
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
            m_vNormals[idx++] = m_normal;
            m_vertices[idx] = calPosition(i, j + 1);
            m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
            m_vNormals[idx++] = m_normal;

            if (j != 0) {
                m_vertices[idx] = calPosition(i, j);
                m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
                m_vNormals[idx++] = m_normal;
                m_vertices[idx] = calPosition(i + 1, j);
                m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
                m_vNormals[idx++] = m_normal;
                m_vertices[idx] = calPosition(i + 1, j + 1);
                m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
                m_vNormals[idx++] = m_normal;
            }
        }
    }
    m_cacheAvailable = true;
}

