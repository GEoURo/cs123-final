#include "MobiusTesselator.h"
#include "glm/gtx/transform.hpp"
#include <iostream>

using namespace std;
using namespace glm;

MobiusTesselator::MobiusTesselator() :
    BaseTesselator(),
    m_stackNum(1),
    m_sliceNum(3)
{

}

void MobiusTesselator::setStackNum(int stackNum) {
    if (stackNum == m_stackNum) {
        return;
    }

    m_stackNum = std::max(1, stackNum);
    m_cacheAvailable = false;
}

void MobiusTesselator::setSliceNum(int sliceNum) {
    if (sliceNum == m_sliceNum) {
        return;
    }

    m_sliceNum = std::max(3, sliceNum);
    m_cacheAvailable = false;
}

int MobiusTesselator::verticesNum() {
    return m_sliceNum * m_stackNum * 2 * 3 * 2;
}

void MobiusTesselator::tesselate() {
    // setup cache if needed
    setup();

    if (m_cacheAvailable) {
        return;
    }

    std::cout << "Mobius tesselate called" << std::endl;

    float thetaStep = 2.f * M_PI / (float)m_sliceNum;
    float widthStep = 1.f / (float)m_stackNum;

    auto calPosition = [thetaStep, widthStep](int i, int j) -> vec3 {
        float theta = thetaStep * i;
        float v = -0.5f + widthStep * j;
        // This is the parametric equation of a mobius strip with R = 1.f
        float x = (1.f + v * glm::cos(0.5 * theta)) * glm::cos(theta);
        float y = (1.f + v * glm::cos(0.5 * theta)) * glm::sin(theta);
        float z = v * glm::sin(0.5 * theta);

        return vec3(x, y, z);
    };

    // Although, mobius strip and sphere both have one curved surface,
    // the mobius is different because both sides of the surface must be drawn.
    // However, both sides are still represented by the same parametric equation,
    // only with opposite normals and inversely arranged triangle dots.

    // We are traversing the surface in a counter clockwise direction, from the inner curve to outer curve.
    // When theta = 0, the math tells us the normal should be parallel to axis z.
    // So we just have to determine whether it's (0, 0, 1) or (0, 0, -1)

    // With the following dots mesh
    // Y
    // | D2, D4
    // | D1, D3
    //  -------- X
    // triangle 1 is (D1, D2, D3), triangle 2 is (D2, D4, D3).

    // Since the triangle strip should be in counter clockwise direction, the normal should be pointing into the screen, which is (0, 0, -1)

    auto calNormal = [thetaStep](int i) -> vec3 {
        // At theta = 0, the normal is pointing towards the -z direction
        vec4 homo = vec4(0.f, 0.f, -1.f, 0.f);

        float theta = i * thetaStep;
        // idea comes from this link: https://math.stackexchange.com/questions/638225/understanding-the-equation-of-a-möbius-strip
        // When sliding on the strip in a counter clockwise direction, the normal is rotating around the y-axis in clockwise direction, hence (-0.5f * theta)
        mat4 rotation1 = glm::rotate(-0.5f * theta, glm::vec3(0.f, 1.f, 0.f));
        mat4 rotation2 = glm::rotate(theta, vec3(0.f, 0.f, 1.f));

        vec4 transformedVec = rotation2 * rotation1 * homo;
        return vec3(transformedVec);
    };

    int idx = 0;

    for (int i = 0;  i < m_sliceNum; i++) {
        for (int j = 0; j < m_stackNum; j++) {
            m_vertices[idx] = calPosition(i, j);
            m_vNormals[idx++] = calNormal(i);
            m_vertices[idx] = calPosition(i + 1, j);
            m_vNormals[idx++] = calNormal(i + 1);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1);

            m_vertices[idx] = calPosition(i, j);
            m_vNormals[idx++] = calNormal(i);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_vNormals[idx++] = calNormal(i + 1);
            m_vertices[idx] = calPosition(i, j + 1);
            m_vNormals[idx++] = calNormal(i);

            // we need to draw both sides of the strip,
            // Thus, we need opposite normals and inversely arranged dots
            m_vertices[idx] = calPosition(i, j);
            m_vNormals[idx++] = -calNormal(i);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_vNormals[idx++] = -calNormal(i + 1);
            m_vertices[idx] = calPosition(i + 1, j);
            m_vNormals[idx++] = -calNormal(i + 1);

            m_vertices[idx] = calPosition(i, j);
            m_vNormals[idx++] = -calNormal(i);
            m_vertices[idx] = calPosition(i, j + 1);
            m_vNormals[idx++] = -calNormal(i);
            m_vertices[idx] = calPosition(i + 1, j + 1);
            m_vNormals[idx++] = -calNormal(i + 1);
        }
    }
}
