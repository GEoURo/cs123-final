#ifndef SQUARETESSLATOR_H
#define SQUARETESSLATOR_H

#include "BaseTesselator.h"

class SquareTesselator : public BaseTesselator
{
public:
    // The constructor of the SquareTesselator,
    // @param origin: The upper left corner of the square.
    // @param normal: The normal vector of the square.
    SquareTesselator(glm::vec3 origin, glm::vec3 normal);

    void setStackNumber(int stackNum);
    void setOrigin(glm::vec3 origin);
    void setNormal(glm::vec3 normal);

    // Tesselate the square with the current configuration.
    void tesselate() override;

protected:
    int verticesNum() override;

private:
    int m_stackNum;
    glm::vec3 m_origin;
    glm::vec3 m_normal;
};

#endif // SQUARETESSLATOR_H
