#include "ConeHatTesselator.h"
#include <math.h>
#include <iostream>

using namespace std;
using namespace glm;

static const float SINE_26 = 1.f / sqrt(5);

ConeHatTesselator::ConeHatTesselator() :
    BaseTesselator(),
    m_stackNum(1),
    m_sliceNum(3)
{

}

void ConeHatTesselator::setStackNum(int stackNum) {
    if (stackNum == m_stackNum) {
        return;
    }

    m_stackNum = std::max(1, stackNum);
    m_cacheAvailable = false;
}

void ConeHatTesselator::setSliceNum(int sliceNum) {
    if (sliceNum == m_sliceNum) {
        return;
    }

    m_sliceNum = std::max(3, sliceNum);
    m_cacheAvailable = false;
}

int ConeHatTesselator::verticesNum() {
    return m_sliceNum * (2 * m_stackNum - 1) * 3;;
}

void ConeHatTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        return;
    }

    std::cout << "Cone Hat tesselate called" << std::endl;

    float radianStep = 2.f * M_PI / (float)m_sliceNum;
    float radiusStep = 0.5f / (float)m_stackNum;
    float heightStep = 1.f / (float)m_stackNum;

    auto calPosition = [radianStep, radiusStep, heightStep](float i, float j) -> vec3 {
        float r = j * radiusStep;
        float x = r * glm::cos(radianStep * i);
        float z = r * glm::sin(radianStep * i);
        float y = 0.5f - j * heightStep;

        return vec3(x, y, z);
    };

    auto calNormal = [radianStep](int i, int j) -> vec3 {
        vec3 normal;
        float r = SINE_26 * 2;
        float x = 0.f, z = 0.f;
        if (j == 0) {
            x = r * glm::cos(radianStep * (i + 0.5f));
            z = r * glm::sin(radianStep * (i + 0.5f));
        } else {
            x = r * glm::cos(radianStep * i);
            z = r * glm::sin(radianStep * i);
            normal = vec3(x, SINE_26, z);
        }
        normal = vec3(x, SINE_26, z);
        return normal;
    };

    auto calTextureUV = [radianStep, heightStep](int i, int j) -> vec2 {
        float theta = i * radianStep;

        // i is going in clockwise direction
        // while the texture mapping for u usually goes in counter clockwise direction,
        // so we need to flip the u value
        float u = 1.f - theta / (2 * M_PI);
        float v = j * heightStep;

        return vec2(u, v);
    };

    int idx = 0;
    for (int i = 0; i < m_sliceNum; i++) {
        for (int j = 0; j < m_stackNum; j++) {
            // The traverse is going in the clockwise direction, from tip to bottom
            // The arrangement of vertices is similar to "Circle"
            m_vertices[idx] = calPosition(i, j);
            m_uvCoords[idx] = calTextureUV(i, j);
            m_vNormals[idx++] = calNormal(i, j);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_uvCoords[idx] = calTextureUV(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1, j + 1);
            m_vertices[idx] = calPosition(i, j + 1);
            m_uvCoords[idx] = calTextureUV(i, j + 1);
            m_vNormals[idx++] = calNormal(i, j + 1);

            if (j != 0) {
                m_vertices[idx] = calPosition(i, j);
                m_uvCoords[idx] = calTextureUV(i, j);
                m_vNormals[idx++] = calNormal(i, j);
                m_vertices[idx] = calPosition(i + 1, j);
                m_uvCoords[idx] = calTextureUV(i + 1, j);
                m_vNormals[idx++] = calNormal(i + 1, j);
                m_vertices[idx] = calPosition(i + 1, j + 1);
                m_uvCoords[idx] = calTextureUV(i + 1, j + 1);
                m_vNormals[idx++] = calNormal(i + 1, j + 1);
            }
        }
    }
    m_cacheAvailable = true;
}
