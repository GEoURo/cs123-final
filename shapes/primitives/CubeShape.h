#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include <memory>
#include "BaseShape.h"

class SquareTesselator;

class CubeShape : public BaseShape
{
public:
    CubeShape(int stackNum);
    ~CubeShape();

    void update(int param1, int param2, float param3, bool force) override;

protected:
    void construct() override;

private:
    int m_stackNum;

    // The position of origins and normal vectors of each face of the square.
    std::vector<glm::vec3> facetOrigins;
    std::vector<glm::vec3> facetNormals;

    std::unique_ptr<SquareTesselator> m_tesselator;

};

#endif // CUBESHAPE_H
