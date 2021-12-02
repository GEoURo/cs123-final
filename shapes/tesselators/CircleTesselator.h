#ifndef CIRCLETESSELATOR_H
#define CIRCLETESSELATOR_H

#include "BaseTesselator.h"

class CircleTesselator : public BaseTesselator
{
public:
    CircleTesselator(glm::vec3 center, glm::vec3 normal);

    void setStackNum(int stackNum);
    void setSliceNum(int sliceNum);

   // Set the center of the circle.
    void setCenter(glm::vec3 center);

    // Set the normal of the circle. (To which direction does the circle points)
    void setNormal(glm::vec3 normal);

    void tesselate() override;

protected:
    int verticesNum() override;

private:
    glm::vec3 m_center;
    glm::vec3 m_normal;

    int m_stackNum;
    int m_sliceNum;
};

#endif // CIRCLETESSELATOR_H
