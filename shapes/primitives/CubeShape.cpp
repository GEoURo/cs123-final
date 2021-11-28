#include "CubeShape.h"
#include "shapes/tesselators/SquareTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

CubeShape::CubeShape(int stackNum) :
    BaseShape(),
    m_stackNum(std::max(1, stackNum)),
    facetOrigins(vector<vec3>(6)),
    facetNormals(vector<vec3>(6)),
    m_tesselator(unique_ptr<SquareTesselator>(new SquareTesselator(vec3(-0.5, 0.5, -0.5), vec3(0.f, 1.f, 0.f))))

{
    // The origins of six different facets of the cube
    facetOrigins[0] = {-0.5f, 0.5f, -0.5f};
    facetOrigins[1] = {-0.5f, -0.5f, 0.5f};
    facetOrigins[2] = {-0.5f, 0.5f, 0.5f};
    facetOrigins[3] = {0.5f, 0.5f, -0.5f};
    facetOrigins[4] = {0.5f, 0.5f, 0.5f};
    facetOrigins[5] = {-0.5f, 0.5f, -0.5f};

    // The normal vectors of six different facets of the cube
    facetNormals[0] = {0.f, 1.f, 0.f};
    facetNormals[1] = {0.f, -1.f, 0.f};
    facetNormals[2] = {0.f, 0.f, 1.f};
    facetNormals[3] = {0.f, 0.f, -1.f};
    facetNormals[4] = {1.f, 0.f, 0.f};
    facetNormals[5] = {-1.f, 0.f, 0.f};
}

CubeShape::~CubeShape() {
    std::cout << "CUBE DESTRUCTOR" << std::endl;
}

void CubeShape::update(int param1, int param2, float param3, bool force) {
    param1 = std::max(1, param1);

    if (!force && param1 == this->m_stackNum) {
        return;
    }

    m_stackNum = param1;
    construct();
}

void CubeShape::construct() {
    m_vertexData.clear();

    m_tesselator->setStackNumber(m_stackNum);

    // tesselate each face
    for (int i = 0; i < 6; i++) {
        m_tesselator->setNormal(facetNormals[i]);
        m_tesselator->setOrigin(facetOrigins[i]);

        m_tesselator->tesselate();

        dump(m_tesselator->getVertices(), m_tesselator->getVNormals(), m_tesselator->getUVCoords());
    }

    initializeOpenGLShapeProperties(true);
}
