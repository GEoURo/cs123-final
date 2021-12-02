#include "SphereShape.h"
#include "shapes/tesselators/SphereTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

SphereShape::SphereShape(int stackNum, int sliceNum) :
    BaseShape(),
    m_stackNum(stackNum),
    m_sliceNum(sliceNum),
    m_sphereTess(unique_ptr<SphereTesselator>(new SphereTesselator()))
{

}

SphereShape::~SphereShape() {
    std::cout << "SPHERE DESTRUCTOR" << std::endl;
}

void SphereShape::update(int param1, int param2, float param3, bool force) {
    param1 = std::max(param1, 2);
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

void SphereShape::construct() {
    m_vertexData.clear();

    m_sphereTess->setStackNum(m_stackNum);
    m_sphereTess->setSliceNum(m_sliceNum);

    m_sphereTess->tesselate();

    dump(m_sphereTess->getVertices(), m_sphereTess->getVNormals(), m_sphereTess->getUVCoords());

    initializeOpenGLShapeProperties(true);
}
