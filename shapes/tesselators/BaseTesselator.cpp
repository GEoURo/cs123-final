#include "BaseTesselator.h"
#include <iostream>

using namespace std;
using namespace glm;

BaseTesselator::BaseTesselator() :
    m_cacheAvailable(false),
    m_vertices(vector<vec3>()),
    m_vNormals(vector<vec3>())
{

}

BaseTesselator::~BaseTesselator() {
    std::cout << "BASE Tesselator DESTRUCTOR" << std::endl;
}

std::vector<glm::vec3> BaseTesselator::getVertices() const {
    return m_vertices;
}

std::vector<glm::vec3> BaseTesselator::getVNormals() const {
    return m_vNormals;
}

std::vector<glm::vec2> BaseTesselator::getUVCoords() const {
    return m_uvCoords;
}

void BaseTesselator::setup() {
    int vNum = verticesNum();
    if (vNum == m_vertices.size()) {
        return;
    }

    m_vertices.resize(vNum);
    m_vNormals.resize(vNum);
    m_uvCoords.resize(vNum);
    m_cacheAvailable = false;
}
