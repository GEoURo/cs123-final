#include "BaseShape.h"
#include <iostream>

BaseShape::BaseShape() :
    m_param1(0),
    m_param2(0),
    m_param3(0)
{

}

BaseShape::~BaseShape() {
    std::cout << "BASE Shape DESTRUCTOR" << std::endl;
}

void BaseShape::dump(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vNormals) {
    if (vertices.size() != vNormals.size()) {
        std::cerr << "dump error: vertices and vNormals have different size" << std::endl;
        return;
    }

    int size = (int)vertices.size();
    for (int i = 0; i < size; i++) {
        insertVec3(this->m_vertexData, vertices[i]);
        insertVec3(this->m_vertexData, vNormals[i]);
    }
    return;
}

void BaseShape::dump(std::vector<glm::vec3> &&vertices, std::vector<glm::vec3> &&vNormals) {
    if (vertices.size() != vNormals.size()) {
        std::cerr << "dump error: vertices and vNormals have different size" << std::endl;
        return;
    }

    int size = (int)vertices.size();
    for (int i = 0; i < size; i++) {
        insertVec3(this->m_vertexData, vertices[i]);
        insertVec3(this->m_vertexData, vNormals[i]);
    }
    return;
}

void BaseShape::dump(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vNormals, std::vector<glm::vec2> &textureUVs) {
    if (vertices.size() != vNormals.size() || textureUVs.size() != vertices.size()) {
        std::cerr << "dump error: vertices and vNormals have different size" << std::endl;
        return;
    }

    int size = (int)vertices.size();
    for (int i = 0; i < size; i++) {
        insertVec3(this->m_vertexData, vertices[i]);
        insertVec3(this->m_vertexData, vNormals[i]);
        insertVec2(this->m_vertexData, textureUVs[i]);
    }
    return;
}

void BaseShape::dump(std::vector<glm::vec3> &&vertices, std::vector<glm::vec3> &&vNormals, std::vector<glm::vec2> &&textureUVs) {
    if (vertices.size() != vNormals.size() || textureUVs.size() != vertices.size()) {
        std::cerr << "dump error: vertices and vNormals have different size" << std::endl;
        return;
    }

    int size = (int)vertices.size();
    for (int i = 0; i < size; i++) {
        insertVec3(this->m_vertexData, vertices[i]);
        insertVec3(this->m_vertexData, vNormals[i]);
        insertVec2(this->m_vertexData, textureUVs[i]);
    }
    return;
}
