#include "TorusTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

TorusTesselator::TorusTesselator() :
    BaseTesselator(),
    m_ringSlice(3),
    m_barrelSlice(3),
    m_radiusPercent(1.f)
{

}

void TorusTesselator::setRingSlice(int ringSlice) {
    if (ringSlice == m_ringSlice) {
        return;
    }

    m_ringSlice = std::max(3, ringSlice);
    m_cacheAvailable = false;
}

void TorusTesselator::setBarrelSlice(int barrelSlice) {
    if (barrelSlice == m_barrelSlice) {
        return;
    }

    m_barrelSlice = std::max(3, barrelSlice);
    m_cacheAvailable = false;
}

void TorusTesselator::setRadiusPercent(float radiusPercent) {
    if (radiusPercent == m_radiusPercent) {
        return;
    }

    m_radiusPercent = std::min(std::max(1.f, radiusPercent), 100.f);
    m_cacheAvailable = false;
}

int TorusTesselator::verticesNum() {
    return m_ringSlice * m_barrelSlice * 2 * 3;
}

void TorusTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        return;
    }

    std::cout << "Torus tesselate called" << std::endl;

    float thetaStep = 2.f * M_PI / (float)m_ringSlice;
    float phiStep = 2.f * M_PI / (float)m_barrelSlice;

    float torusRadius = 0.5f * m_radiusPercent * 0.01f;
    float torusCenter = 0.5f - torusRadius;

    auto calPosition = [torusCenter, torusRadius, thetaStep, phiStep](int i, int j) -> vec3 {
        float theta = i * thetaStep, phi = j * phiStep;
        // A parametric equation of the torus
        float x = (torusCenter + torusRadius * glm::cos(phi)) * glm::cos(theta);
        float y = torusRadius * glm::sin(phi);
        float z = (torusCenter + torusRadius * glm::cos(phi)) * glm::sin(theta);
        return vec3(x, y, z);
    };

    auto calNormal = [thetaStep, phiStep](int i, int j) -> vec3 {
        float theta = i * thetaStep, phi = j * phiStep;
        // The parametric equation without scale and offsets
        float x = glm::cos(phi) * glm::cos(theta);
        float y = glm::sin(phi);
        float z = glm::cos(phi) * glm::sin(theta);
        return vec3(x, y, z);
    };

    int idx = 0;
    for (int i = 0; i < m_ringSlice; i++) {
        for (int j = 0; j < m_barrelSlice; j++) {
            // The traverse is going in clockwise direction of the torus,
            // and also in counter clockwise direction on the torus surface.
            // When phi is zero, the dot is on the outer circle,
            m_vertices[idx] = calPosition(i, j);
            m_vNormals[idx++] = calNormal(i, j);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1, j + 1);
            m_vertices[idx] = calPosition(i + 1, j);
            m_vNormals[idx++] = calNormal(i + 1, j);

            m_vertices[idx] = calPosition(i, j + 1);
            m_vNormals[idx++] = calNormal(i, j + 1);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1, j + 1);
            m_vertices[idx] = calPosition(i, j);
            m_vNormals[idx++] = calNormal(i, j);
        }
    }
}

