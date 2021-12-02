#include "CylinderShape.h"
#include "shapes/tesselators/BarrelTesselator.h"
#include "shapes/tesselators/CircleTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

CylinderShape::CylinderShape(int stackNum, int sliceNum) :
    BaseShape(),
    m_stackNum(std::max(stackNum, 1)),
    m_sliceNum(std::max(sliceNum, 3)),
    m_barrelTess(unique_ptr<BarrelTesselator>(new BarrelTesselator())),
    m_circleTess(unique_ptr<CircleTesselator>(new CircleTesselator(vec3(0.f, -0.5f, 0.f), vec3(0.f, -1.f, 0.f)))),
    m_capCenters(vector<vec3>(2)),
    m_capNormals(vector<vec3>(2))
{
    m_capCenters[0] = {0.f, -0.5f, 0.f};
    m_capCenters[1] = {0.f, 0.5f, 0.f};

    m_capNormals[0] = {0.f, -1.f, 0.f};
    m_capNormals[1] = {0.f, 1.f, 0.f};
}

CylinderShape::~CylinderShape() {
    std::cout << "CYLINDER DESTRUCTOR" << std::endl;
}

void CylinderShape::update(int param1, int param2, float param3, bool force) {
    param1 = std::max(param1, 1);
    param2 = std::max(param2, 3);

    if (!force &&
        param1 == this->m_stackNum &&
        param2 == this->m_sliceNum) {
        return;
    }

    m_stackNum = param1;
    m_sliceNum = param2;
    construct();
}

void CylinderShape::construct() {
    m_vertexData.clear();

    // tesselate the barrel
    m_barrelTess->setStackNum(m_stackNum);
    m_barrelTess->setSliceNum(m_sliceNum);

    m_barrelTess->tesselate();

    dump(m_barrelTess->getVertices(), m_barrelTess->getVNormals(), m_barrelTess->getUVCoords());

    m_circleTess->setStackNum(m_stackNum);
    m_circleTess->setSliceNum(m_sliceNum);

    // tesselate the top and bottom cap
    for (int i = 0; i < 2; i++) {
        m_circleTess->setCenter(m_capCenters[i]);
        m_circleTess->setNormal(m_capNormals[i]);

        m_circleTess->tesselate();

        dump(m_circleTess->getVertices(), m_circleTess->getVNormals(), m_circleTess->getUVCoords());
    }

    initializeOpenGLShapeProperties(true);
}
