#include "MobiusShape.h"
#include "shapes/tesselators/MobiusTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

MobiusShape::MobiusShape(int stackNum, int sliceNum) :
    BaseShape(),
    m_stackNum(stackNum),
    m_sliceNum(sliceNum),
    m_mobiusTess(unique_ptr<MobiusTesselator>(new MobiusTesselator()))

{

}

MobiusShape::~MobiusShape() {
    std::cout << "MOBIUS DESTRUCTOR" << std::endl;
}

void MobiusShape::update(int param1, int param2, float param3, bool force) {
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

void MobiusShape::construct() {
    m_vertexData.clear();

    m_mobiusTess->setStackNum(m_stackNum);
    m_mobiusTess->setSliceNum(m_sliceNum);

    m_mobiusTess->tesselate();

    dump(m_mobiusTess->getVertices(), m_mobiusTess->getVNormals());

    initializeOpenGLShapeProperties();
}
