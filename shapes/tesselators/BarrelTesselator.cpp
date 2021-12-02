#include "BarrelTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

BarrelTesselator::BarrelTesselator() :
    BaseTesselator(),
    m_stackNum(1),
    m_sliceNum(3)
{

}

void BarrelTesselator::setStackNum(int stackNum) {
    if (stackNum == m_stackNum) {
        return;
    }

    m_stackNum = std::max(1, stackNum);
    m_cacheAvailable = false;
}

void BarrelTesselator::setSliceNum(int sliceNum) {
    if (sliceNum == m_sliceNum) {
        return;
    }

    m_sliceNum = std::max(3, sliceNum);
    m_cacheAvailable = false;
}

int BarrelTesselator::verticesNum() {
    return m_sliceNum * m_stackNum * 2 * 3;
}

void BarrelTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        return;
    }

    std::cout << "Barrel tesselate called" << std::endl;

    float radianStep = 2.f * M_PI / (float)m_sliceNum;
    float heightStep = 1.f / m_stackNum;

    auto calPosition = [radianStep, heightStep](float i, float j) -> vec3 {
        float height = 0.5 - j * heightStep;
        vec3 ret = vec3(0.5 * glm::cos(radianStep * i), height, 0.5 * glm::sin(radianStep * i));
        return ret;
    };

    auto calNormal = [radianStep](float i) -> vec3 {
        return vec3(glm::cos(radianStep * i), 0.f, glm::sin(radianStep * i));
    };

    auto calTextureUV = [radianStep, heightStep](int i, int j) -> vec2 {
        float theta = i * radianStep;

        // i is going in clockwise direction
        // while the texture mapping for u usually goes in counter clockwise direction,
        // so we need to flip the u value
        float v = j * heightStep;
        float u = 1.f - theta / (2 * M_PI);

        return vec2(u, v);
    };

    int idx = 0;
    for (int i = 0; i < m_sliceNum; i++) {
        for (int j = 0; j < m_stackNum; j++) {
            // The traverse is going in the clockwise direction, from top to bottom
            m_vertices[idx] = calPosition(i, j);
            m_uvCoords[idx] = calTextureUV(i, j);
            m_vNormals[idx++] = calNormal(i);
            m_vertices[idx] = calPosition(i + 1, j);
            m_uvCoords[idx] = calTextureUV(i + 1, j);
            m_vNormals[idx++] = calNormal(i + 1);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_uvCoords[idx] = calTextureUV(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1);

            m_vertices[idx] = calPosition(i, j);
            m_uvCoords[idx] = calTextureUV(i, j);
            m_vNormals[idx++] = calNormal(i);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_uvCoords[idx] = calTextureUV(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1);
            m_vertices[idx] = calPosition(i, j + 1);
            m_uvCoords[idx] = calTextureUV(i, j + 1);
            m_vNormals[idx++] = calNormal(i);
        }
    }
}
