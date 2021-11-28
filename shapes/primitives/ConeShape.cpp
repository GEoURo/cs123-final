#include "ConeShape.h"
#include "shapes/tesselators/CircleTesselator.h"
#include "shapes/tesselators/ConeHatTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

ConeShape::ConeShape(int stackNum, int sliceNum) :
    BaseShape(),
    m_stackNum(std::max(stackNum, 1)),
    m_sliceNum(std::max(sliceNum, 3)),
    m_hatTess(unique_ptr<ConeHatTesselator>(new ConeHatTesselator())),
    m_circleTess(unique_ptr<CircleTesselator>(new CircleTesselator(vec3(0.f, -0.5f, 0.f), vec3(0.f, -1.f, 0.f))))
{

}

ConeShape::~ConeShape() {
    std::cout << "CONE DESTRUCTOR" << std::endl;
}

void ConeShape::update(int param1, int param2, float param3, bool force) {
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

void ConeShape::construct() {
    m_vertexData.clear();

    // tesselate the bottom cap
    m_circleTess->setStackNum(m_stackNum);
    m_circleTess->setSliceNum(m_sliceNum);

    m_circleTess->tesselate();

    dump(m_circleTess->getVertices(), m_circleTess->getVNormals(), m_circleTess->getUVCoords());

    // tesselate the hat
    m_hatTess->setStackNum(m_stackNum);
    m_hatTess->setSliceNum(m_sliceNum);

    m_hatTess->tesselate();

    dump(m_hatTess->getVertices(), m_hatTess->getVNormals(), m_hatTess->getUVCoords());

    initializeOpenGLShapeProperties(true);
}
