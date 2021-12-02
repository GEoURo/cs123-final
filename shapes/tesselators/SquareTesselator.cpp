#include "SquareTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

SquareTesselator::SquareTesselator(glm::vec3 origin, glm::vec3 normal) :
    BaseTesselator(),
    m_stackNum(1),
    m_origin(origin),
    m_normal(normal)
{

}

void SquareTesselator::setStackNumber(int stackNum) {
    if (stackNum == m_stackNum) {
        return;
    }

    m_stackNum = std::max(1, stackNum);
    m_cacheAvailable = false;
}

void SquareTesselator::setOrigin(glm::vec3 origin) {
    if (origin == m_origin) {
        return;
    }

    m_origin = origin;
    m_cacheAvailable = false;
}

void SquareTesselator::setNormal(glm::vec3 normal) {
    if (normal == m_normal) {
        return;
    }

    m_normal = normal;
    m_cacheAvailable = false;
}

int SquareTesselator::verticesNum() {
    return m_stackNum * m_stackNum * 6;;
}

void SquareTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        // the config doesn't change, then there is no need for tesselation
        return;
    }

    std::cout << "Square tesselate called" << std::endl;

    float dx = 0.f, dy = 0.f, dz = 0.f;
    float stepLen = 1.f / m_stackNum;

    // Here, we are assuming the m_normal is parallel to one of the three axises.

    // The iPrime and jPrime is a mask indicating which axis is i or j traversing.
    // For example, if iPrime = {1, 0, 0}, then i is traversing the x axis.

    // The (-m_origin.x) / abs(m_origin.x) is determing which direction is dx moving.
    // Given the fact that the origin is the upper left point of the square when viewing from its front,
    // and the square is centered at {0, 0, 0}, we are definitely moving dx across zero.
    // That's why (-m_origin.x) / abs(m_origin.x) gives the direction

    vec3 iPrime, jPrime;
    if (m_normal.x != 0.) {
        // parallel to x axis
        dy = stepLen * (-m_origin.y) / abs(m_origin.y);
        dz = stepLen * (-m_origin.z) / abs(m_origin.z);
        iPrime = {0.f, 1.f, 0.f};
        jPrime = {0.f, 0.f, 1.f};
    } else if (m_normal.y != 0.) {
        // parallel to y axis
        dx = stepLen * (-m_origin.x) / abs(m_origin.x);
        dz = stepLen * (-m_origin.z) / abs(m_origin.z);
        iPrime = {0.f, 0.f, 1.f};
        jPrime = {1.f, 0.f, 0.f};
    } else {
        // parallel to z axis
        dx = stepLen * (-m_origin.x) / abs(m_origin.x);
        dy = stepLen * (-m_origin.y) / abs(m_origin.y);
        iPrime = {0.f, 1.f, 0.f};
        jPrime = {1.f, 0.f, 0.f};
    }

    vec3 stepLenV = vec3(dx, dy, dz);

    auto calPosition = [&iPrime, &jPrime, &stepLenV, this](float i, float j) -> vec3 {
        vec3 steps = i * iPrime + j * jPrime;
        return m_origin + steps * stepLenV;;
    };

    auto calTextureUV = [this](vec3 &pos) -> vec2 {
        float u = 0.f, v = 0.f;
        if (m_normal.x == 1.f) {
                u = 0.5f - pos.z;
                v = 0.5f - pos.y;
        } else if (m_normal.x == -1.f) {
                u = pos.z + 0.5f;
                v = 0.5f - pos.y;
        } else if (m_normal.y == 1.f) {
            u = pos.x + 0.5f;
            v = pos.z + 0.5f;
        } else if (m_normal.y == -1.f) {
            u = pos.x + 0.5f;
            v = 0.5f - pos.z;
        } else if (m_normal.z == 1.f) {
            u = pos.x + 0.5f;
            v = 0.5f - pos.y;
        } else {
            u = 0.5f - pos.x;
            v = 0.5f - pos.y;
        }

        return vec2(u, v);
    };

    int idx = 0;
    for (int i = 0; i < m_stackNum; i++) {
        for (int j = 0; j < m_stackNum; j++) {
            // The traverse is going from top left to bottom right.
            m_vertices[idx] = calPosition(i, j);
            m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
            m_vNormals[idx++] = m_normal;
            m_vertices[idx] = calPosition(i + 1, j);
            m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
            m_vNormals[idx++] = m_normal;
            m_vertices[idx] = calPosition(i, j + 1);
            m_uvCoords[idx] = calTextureUV(m_vertices[idx]);
            m_vNormals[idx++] = m_normal;

            m_vertices[idx] = calPosition(i, j + 1);
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
    m_cacheAvailable = true;
}
