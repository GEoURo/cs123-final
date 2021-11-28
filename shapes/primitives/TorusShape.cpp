#include "TorusShape.h"
#include "shapes/tesselators/TorusTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

TorusShape::TorusShape(int ringSlice, int barrelSlice, float radiusPercent) :
    BaseShape(),
    m_ringSlice(ringSlice),
    m_barrelSlice(barrelSlice),
    m_radiusPercent(radiusPercent),
    m_torusTess(unique_ptr<TorusTesselator>(new TorusTesselator()))
{

}

TorusShape::~TorusShape() {
    std::cout << "TORUS DESTRUCTOR" << std::endl;
}

void TorusShape::update(int param1, int param2, float param3, bool force) {
    param1 = std::max(param1, 3);
    param2 = std::max(param2, 3);
    param3 = std::min(std::max(1.f, param3), 100.f);

    if (!force &&
        param1 == this->m_ringSlice &&
        param2 == this->m_barrelSlice &&
        param3 == this->m_radiusPercent) {
        return;
    }

    m_ringSlice = param1;
    m_barrelSlice = param2;
    m_radiusPercent = param3;
    construct();
}

void TorusShape::construct() {
    m_vertexData.clear();

    m_torusTess->setRingSlice(m_ringSlice);
    m_torusTess->setBarrelSlice(m_barrelSlice);
    m_torusTess->setRadiusPercent(m_radiusPercent);

    m_torusTess->tesselate();

    dump(m_torusTess->getVertices(), m_torusTess->getVNormals());

    initializeOpenGLShapeProperties();
}
