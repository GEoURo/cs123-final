#ifndef BASESHAPE_H
#define BASESHAPE_H

#include "OpenGLShape.h"

class BaseShape : public OpenGLShape
{
public:
    BaseShape();
    virtual ~BaseShape();

    // The function that updates the shape with the new parameters
    virtual void update(int param1, int param2, float param3, bool force) = 0;

protected:
    // The function that constructs the shape.
    virtual void construct() = 0;

    // The dump functions take vertices array and normal vectors array and put them into "m_vertexData"
    void dump(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vNormals);
    void dump(std::vector<glm::vec3> &&vertices, std::vector<glm::vec3> &&vNormals);
    void dump(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vNormals, std::vector<glm::vec2> &textureUVs);
    void dump(std::vector<glm::vec3> &&vertices, std::vector<glm::vec3> &&vNormals, std::vector<glm::vec2> &&textureUVs);

private:
    int m_param1;
    int m_param2;
    int m_param3;
};

#endif // BASESHAPE_H
