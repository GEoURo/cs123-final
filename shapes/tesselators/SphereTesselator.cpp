#include "SphereTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

SphereTesselator::SphereTesselator() :
    BaseTesselator(),
    m_stackNum(2),
    m_sliceNum(3)
{

}

void SphereTesselator::setStackNum(int stackNum) {
    if (stackNum == m_stackNum) {
        return;
    }

    m_stackNum = std::max(2, stackNum);
    m_cacheAvailable = false;
}

void SphereTesselator::setSliceNum(int sliceNum) {
    if (sliceNum == m_sliceNum) {
        return;
    }

    m_sliceNum = std::max(3, sliceNum);
    m_cacheAvailable = false;
}

int SphereTesselator::verticesNum() {
    return m_sliceNum * (m_stackNum - 1) * 2 * 3;
}

void SphereTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        return;
    }

    std::cout << "Sphere tesselate called" << std::endl;

    float longitudeStep = 2 * M_PI / (float)m_sliceNum;
    float latitudeStep = M_PI / (float)m_stackNum;

    auto calNormal = [&longitudeStep, &latitudeStep](int i, int j) -> vec3 {
        float theta = i * longitudeStep;
        float phi = j * latitudeStep;
        // The parametric equation of a sphere
        float x = glm::sin(phi) * glm::cos(theta);
        float y = glm::cos(phi);
        float z = glm::sin(phi) * glm::sin(theta);
        return vec3(x, y, z);
    };

    auto calTextureUV = [longitudeStep, latitudeStep](int i, int j) -> vec2 {
        float phi = j * latitudeStep;
        float theta = i * longitudeStep;

        // i is going in clockwise direction
        // while the texture mapping for u usually goes in counter clockwise direction,
        // so we need to flip the u value
        float u = 1.f - theta / (2 * M_PI);
        float v = phi / M_PI;

        return vec2(u, v);
    };

    int idx = 0;
    for (int i = 0; i < m_sliceNum; i++) {
        for (int j = 0; j < m_stackNum; j++) {
            // The traverse is going in the clockwise direction, from north pole to south pole
            vec3 cur = calNormal(i, j);
            vec3 curLeft = calNormal(i + 1, j);
            vec3 curLeftBelow = calNormal(i + 1, j + 1);
            vec3 curBelow = calNormal(i, j + 1);

            vec2 curUV = calTextureUV(i, j);
            vec2 curLeftUV = calTextureUV(i + 1, j);
            vec2 curLeftBelowUV = calTextureUV(i + 1, j + 1);
            vec2 curBelowUV = calTextureUV(i, j + 1);

            if (j != 0) {
                m_vertices[idx] = cur * 0.5f;
                m_uvCoords[idx] = curUV;
                m_vNormals[idx++] = cur;
                m_vertices[idx] = curLeft * 0.5f;
                m_uvCoords[idx] = curLeftUV;
                m_vNormals[idx++] = curLeft;
                m_vertices[idx] = curBelow * 0.5f;
                m_uvCoords[idx] = curBelowUV;
                m_vNormals[idx++] = curBelow;
            }

            if (j != m_stackNum - 1) {
                m_vertices[idx] = curLeft * 0.5f;
                m_uvCoords[idx] = curLeftUV;
                m_vNormals[idx++] = curLeft;
                m_vertices[idx] = curLeftBelow * 0.5f;
                m_uvCoords[idx] = curLeftBelowUV;
                m_vNormals[idx++] = curLeftBelow;
                m_vertices[idx] = curBelow * 0.5f;
                m_uvCoords[idx] = curBelowUV;
                m_vNormals[idx++] = curBelow;
            }
        }
    }
    m_cacheAvailable = true;
}


